#include "Module_CustomShader.h"
#include <Cool/Camera/CameraShaderU.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/InputParser/InputParser.h>
#include <Cool/Log/ToUser.h>
#include <Cool/String/String.h>
#include <concepts>
#include <glpp/glpp.hpp>
#include <ranges>
#include <sstream>
#include "Common/make_shader_compilation_error_message.h"

// TODO(LD) Support angle units (turns, degrees, radians)

namespace Lab {

Module_CustomShader::Module_CustomShader(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::InputFactory_Ref input_factory)
    : Module{"Custom Shader", dirty_flag_factory}
    , _shader{dirty_flag_factory.make()}
    , _camera_input{input_factory.make<Cool::Camera>(dirty_flag(), "Camera")}
    , _file{_shader.dirty_flag()}
{
}

static auto settings_from_inputs(
    const std::vector<Cool::AnyInput>& inputs,
    const Cool::VariableRegistries&    registry
) -> std::vector<Cool::AnyVariable>
{
    std::vector<Cool::AnyVariable> settings;
    settings.reserve(inputs.size());
    for (const auto& input : inputs)
    {
        const auto maybe_variable = std::visit([&](auto&& input) -> std::optional<Cool::AnyVariable> {
            const auto maybe_variable = registry.get(input._default_variable_id);
            if (maybe_variable)
            {
                return Cool::AnyVariable{*maybe_variable};
            }
            else
            {
                return std::nullopt;
            }
        },
                                               input);
        if (maybe_variable)
        {
            settings.push_back(*maybe_variable);
        }
    }
    return settings;
}

template<typename T>
auto get_concrete_variable(const Cool::Input<T>&, const Cool::AnyVariable& var) -> Cool::Variable<T>
{
    return std::get<Cool::Variable<T>>(var);
}

static void apply_settings_to_inputs(
    const std::vector<Cool::AnyVariable>& settings,
    std::vector<Cool::AnyInput>&          inputs,
    Cool::VariableRegistries&             registry
)
{
    try
    {
        for (size_t i = 0; i < inputs.size(); ++i)
        {
            std::visit([&](auto&& input) {
                registry.set(
                    input._default_variable_id,
                    get_concrete_variable(input, settings.at(i))
                );
            },
                       inputs.at(i));
        }
    }
    catch (...)
    {
        // TODO(JF) Remove this try-catch once we update presets properly
        Cool::Log::ToUser::warning("Presets", "This preset does not match the INPUTs of the shader anymore, it has not been applied fully.");
    }
}

static void modify_default_variables_of_the_inputs(
    std::vector<Cool::AnyInput>&           inputs,
    Cool::VariableRegistries&              registry,
    std::invocable<Cool::Settings&> auto&& callback
)
{
    // Get the variables from the inputs
    auto settings = settings_from_inputs(inputs, registry);
    // Apply
    callback(settings);
    // Apply back the variables to the inputs' default variables
    apply_settings_to_inputs(settings, inputs, registry);
}

void Module_CustomShader::imgui_windows(Ui_Ref ui) const
{
    Ui_Ref::window({.name = "Custom Shader"}, [&]() {
        Cool::ImGuiExtras::bring_attention_if(_shader_compilation_error.should_highlight(), [&]() {
            ui.widget(_file);
        });
        ImGui::Separator();
        ImGui::NewLine();
        for (auto& input : inputs())
        {
            std::visit([&ui](auto&& input) {
                ui.widget(input);
            },
                       input);
        }
        if (_presets_manager)
        {
            ImGui::Separator();
            modify_default_variables_of_the_inputs(inputs(), ui.variable_registries(), [&](Cool::Settings& settings) {
                _presets_manager->imgui_presets(settings);
            });
        }
    });
}

template<typename T>
static void set_uniform(const Cool::OpenGL::Shader& shader, std::string_view name, const T& value)
{
    shader.set_uniform(name, value);
}

template<>
void set_uniform(const Cool::OpenGL::Shader& shader, std::string_view name, const Cool::RgbColor& value)
{
    shader.set_uniform(name, value.value);
}

void set_uniform(const Cool::OpenGL::Shader&, std::string_view, const Cool::Camera&)
{
    assert(false); // This isn't used at the moment because we set the camera3d manually for all shaders, but this should be changed
    // Cool::CameraShaderU::set_uniform(shader, value);
}

void Module_CustomShader::render(RenderParams in, UpdateContext_Ref update_ctx)
{
    refresh_pipeline_if_necessary(in.provider, in.is_dirty, in.input_factory, in.input_destructor, update_ctx, in.variable_registries);
    if (!_shader.pipeline().shader())
        return;

    _shader.pipeline().shader()->bind();
    _shader.pipeline().shader()->set_uniform("_aspect_ratio", in.provider(Cool::Input_AspectRatio{}));
    _shader.pipeline().shader()->set_uniform("_time", in.provider(Cool::Input_Time{}));

    for (auto& input : inputs())
    {
        std::visit([&](auto&& input) {
            set_uniform(*_shader.pipeline().shader(), input.name(), in.provider(input));
        },
                   input);
    }
    Cool::CameraShaderU::set_uniform(*_shader.pipeline().shader(), in.provider(_camera_input), in.provider(Cool::Input_AspectRatio{}));
    _shader.pipeline().draw();
}

static auto preset_path(std::filesystem::path path) -> std::filesystem::path
{
    path.concat(".presets.json");
    return path;
}

void Module_CustomShader::refresh_pipeline_if_necessary(
    Cool::InputProvider_Ref   provider,
    Cool::IsDirty_Ref         is_dirty,
    Cool::InputFactory_Ref    input_factory,
    Cool::InputDestructor_Ref input_destructor,
    UpdateContext_Ref         update_ctx,
    Cool::VariableRegistries& /* variable_registries */
)
{
    // Check dirty flag
    if (!is_dirty(_shader.dirty_flag()))
        return;
    update_ctx.set_clean(_shader.dirty_flag());

    //
    const auto reset_shader = [&]() {
        _previous_path = "";
        _shader.pipeline().reset();
        _presets_manager.reset();
    };

    // Check if file path exists
    const auto current_path = provider(_file);
    if (!Cool::File::exists(current_path.string()))
    {
        reset_shader();
        return;
    }

    // Load shader from file
    const auto source_code = Cool::File::to_string(current_path.string());
    if (!source_code) // Did not manage to read file, it is maybe protected by password.
    {
        _shader_compilation_error.send(
            make_shader_compilation_error_message(name(), current_path.string(), source_code.error())
        );
        reset_shader();
        return;
    }

    // Parse INPUTs
    {
        const auto err = parse_shader_for_inputs(*source_code, input_factory, input_destructor);
        if (err)
        {
            _shader_compilation_error.send(
                make_shader_compilation_error_message(name(), current_path.string(), *err)
            );
            reset_shader();
            return;
        }
    }

    // Update presets manager if the path has changed
    {
        const bool path_has_changed = _previous_path != current_path;
        _previous_path              = current_path;
        if (path_has_changed)
        {
            _presets_manager.emplace(preset_path(current_path));
            // apply_first_preset_if_there_is_one(variable_registries); // TODO(JF) Reintroduce once we have the merger and don't crash on invalid presets
        }
    }

    // Compile the shader
    _shader
        .compile(Cool::preprocess_inputs(*source_code, inputs(), provider), update_ctx)
        .send_error_if_any(_shader_compilation_error, [&](const std::string& msg) {
            return make_shader_compilation_error_message(name(), current_path.string(), msg);
        });
}

void Module_CustomShader::apply_first_preset_if_there_is_one(Cool::VariableRegistries& variable_registries)
{
    modify_default_variables_of_the_inputs(inputs(), variable_registries, [&](Cool::Settings& settings) {
        _presets_manager->apply_first_preset_if_there_is_one(settings);
    });
}

static auto name(const Cool::AnyInput& input)
{
    return std::visit(([](auto&& input) { return input.name(); }), input);
}

static auto inputs_have_the_same_type_and_name(const Cool::AnyInput& input1, const Cool::AnyInput& input2) -> bool
{
    return input1.index() == input2.index()
           && name(input1) == name(input2);
}

static auto iterator_to_same_input(const Cool::AnyInput& input, std::vector<Cool::AnyInput>& old_inputs)
{
    return std::find_if(old_inputs.begin(), old_inputs.end(), [&](const Cool::AnyInput& other_input) {
        return inputs_have_the_same_type_and_name(other_input, input);
    });

    // Waiting for xcode to support std::ranges::find_if
    //  return std::ranges::find_if(old_inputs, [&](const AnyInput& other_input) {
    //      return inputs_have_the_same_type_and_name(other_input, input);
    //  });
}

static void keep_values_of_inputs_that_already_existed_and_destroy_unused_ones(
    std::vector<Cool::AnyInput>& old_inputs,
    std::vector<Cool::AnyInput>& new_inputs,
    Cool::InputDestructor_Ref    destroy
)
{
    for (auto& input : old_inputs)
    {
        const auto it = iterator_to_same_input(input, new_inputs);
        if (it != new_inputs.end())
        {
            auto description = std::visit([](auto&& input) { return std::move(input._description); }, *it); // Keep the newly-parsed description
            destroy(*it);
            *it = std::move(input);
            std::visit([&](auto&& it) mutable { it._description = std::move(description); }, *it);
        }
        else
        {
            destroy(input);
        }
    }
}

auto Module_CustomShader::parse_shader_for_inputs(
    std::string_view          source_code,
    Cool::InputFactory_Ref    input_factory,
    Cool::InputDestructor_Ref input_destructor
) -> std::optional<std::string>
{
    auto new_inputs = Cool::parse_all_inputs(source_code, dirty_flag(), input_factory);
    if (!new_inputs)
        return new_inputs.error();

    keep_values_of_inputs_that_already_existed_and_destroy_unused_ones(inputs(), *new_inputs, input_destructor);
    inputs() = std::move(*new_inputs);

    return std::nullopt;
}

void Module_CustomShader::set_image_in_shader(std::string_view name, int slot, GLuint texture_id)
{
    if (_shader.pipeline().shader())
    {
        _shader.pipeline().shader()->bind();
        glpp::active_texture(slot);
        glpp::bind_texture<glpp::TextureKind::Tex2D>(texture_id);
        glpp::set_minification_filter<glpp::TextureKind::Tex2D>(texture_id, glpp::Interpolation::LinearMipmapLinear); // TODO do this only once at texture creation, and only if the curent custom shader needs it
        glpp::generate_mipmap<glpp::TextureKind::Tex2D>();                                                            // TODO DO this only when the texture changes (aka the corresponsing module gets re-rendered)
        _shader.pipeline().shader()->set_uniform(name, slot);
    }
}

} // namespace Lab
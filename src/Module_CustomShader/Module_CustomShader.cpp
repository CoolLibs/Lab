#include "Module_CustomShader.h"
#include <Cool/Camera/CameraShaderU.h>
#include <Cool/Log/ToUser.h>
#include <Cool/String/String.h>
#include <glpp/glpp.hpp>
#include <ranges>
#include <sstream>
#include <type_from_string/type_from_string.hpp>

namespace Lab {

Module_CustomShader::Module_CustomShader(DirtyFlagFactory_Ref dirty_flag_factory, InputFactory_Ref input_factory)
    : Module{"Custom Shader", dirty_flag_factory}
    , _shader{dirty_flag_factory.make()}
    , _camera_input{input_factory.make<Cool::Camera>(dirty_flag(), "Camera")}
    , _file{_shader.dirty_flag()}
{
}

void Module_CustomShader::imgui_windows(Ui_Ref ui) const
{
    Ui_Ref::window({.name = "Custom Shader"}, [&]() {
        ui.widget(_file);
        ImGui::Separator();
        ImGui::NewLine();
        for (auto& input : _inputs)
        {
            std::visit([&ui](auto&& input) {
                ui.widget(input);
            },
                       input);
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
    refresh_pipeline_if_necessary(in.provider, in.is_dirty, in.input_factory, in.input_destructor, update_ctx);
    if (_shader.pipeline().shader())
    {
        _shader.pipeline().shader()->bind();
        _shader.pipeline().shader()->set_uniform("_aspect_ratio", in.provider(Input_AspectRatio{}));
        _shader.pipeline().shader()->set_uniform("_time", in.provider(Input_Time{}));

        for (auto& input : _inputs)
        {
            std::visit([&](auto&& input) {
                set_uniform(*_shader.pipeline().shader(), input.name(), in.provider(input));
            },
                       input);
        }
        Cool::CameraShaderU::set_uniform(*_shader.pipeline().shader(), in.provider(_camera_input), in.provider(Input_AspectRatio{}));
        _shader.pipeline().draw();
    }
}

void Module_CustomShader::refresh_pipeline_if_necessary(
    InputProvider_Ref   provider,
    IsDirty_Ref         is_dirty,
    InputFactory_Ref    input_factory,
    InputDestructor_Ref input_destructor,
    UpdateContext_Ref   update_ctx
)
{
    if (is_dirty(_shader.dirty_flag()))
    {
        const auto file_path   = provider(_file);
        const auto source_code = Cool::File::to_string(file_path.string());
        _shader.compile(source_code, file_path.string(), name(), update_ctx);
        parse_shader_for_params(source_code, input_factory, input_destructor);
    }
}

static auto name(const AnyInput& input)
{
    return std::visit(([](auto&& input) { return input.name(); }), input);
}

static auto inputs_have_the_same_type_and_name(const AnyInput& input1, const AnyInput& input2) -> bool
{
    return input1.index() == input2.index() &&
           name(input1) == name(input2);
}

static auto iterator_to_same_input(const AnyInput& input, std::vector<AnyInput>& old_inputs)
{
    return std::ranges::find_if(old_inputs, [&](const AnyInput& other_input) {
        return inputs_have_the_same_type_and_name(other_input, input);
    });
}

static void keep_values_of_inputs_that_already_existed_and_destroy_unused_ones(
    std::vector<AnyInput>& old_inputs,
    std::vector<AnyInput>& new_inputs,
    InputDestructor_Ref    destroy
)
{
    for (auto& input : old_inputs)
    {
        const auto it = iterator_to_same_input(input, new_inputs);
        if (it != new_inputs.end())
        {
            *it = std::move(input);
        }
        else
        {
            destroy(input);
        }
    }
}

template<typename T>
auto get_default_value(std::string_view key_values) -> T
{
    const auto default_T = Cool::String::find_value_for_given_key<T>(key_values, "default");
    if (default_T.has_value())
    {
        return *default_T;
    }

    auto default_without_key = Cool::String::find_next_word(key_values, 0);
    if (!default_without_key.has_value())
    {
        return T{};
    }

    auto default_value = Cool::String::value_from_string<T>(*default_without_key);
    if (default_value.has_value())
    {
        return *default_value;
    }
    return T{};
}

template<typename T>
auto get_default_metadata(std::string_view key_values) -> Cool::VariableMetadata<T>
{
    static_assert("Type not already defined, you can add it in generate_variables.py");
}

#include "generated_variables/find_metadatas_in_string.inl"

template<typename T>
auto make_input(
    const std::string_view name,
    DirtyFlag              dirty_flag,
    InputFactory_Ref       input_factory,
    std::string_view       key_values
) -> Input<T>
{
    return input_factory.make<T>(
        dirty_flag,
        name,
        get_default_value<T>(key_values),
        get_default_metadata<T>(key_values)
    );
}

auto make_any_input(
    const std::string_view type,
    const std::string_view name,
    DirtyFlag              dirty_flag,
    InputFactory_Ref       input_factory,
    std::string_view       key_values
)
    -> AnyInput
{
    return TFS_EVALUATE_FUNCTION_TEMPLATE(make_input, type, AnyInput, (name, dirty_flag, input_factory, key_values));
}

void separate_input_elements(std::string line, DirtyFlag dirty_flag, InputFactory_Ref input_factory, std::vector<AnyInput>& new_inputs)
{
    const auto uniform_pos = line.find("uniform");
    if (uniform_pos != std::string::npos)
    {
        const auto comment_pos = line.find("//");
        if (comment_pos < uniform_pos)
        {
            return;
        }

        auto space_before_include = Cool::String::find_next_word_position(line, 0);
        if (!space_before_include)
        {
            return;
        }
        line = Cool::String::replace_at(0, space_before_include->first, line, "");

        std::string uniform     = "uniform";
        size_t      current_pos = uniform.length();

        std::string type = Cool::String::next_word(line, current_pos);

        current_pos += type.length() + 1; // find_next_word_position to find current_pos even if there are whitespaces between words

        std::string name = Cool::String::next_word(line, current_pos);

        // const std::vector<std::pair<std::string, std::string>> replacements = {
        //     std::make_pair("_", " "),
        // };

        // Cool::String::replace_all(name, "_", " ");

        const auto key_values = [&]() -> std::string {
            if (comment_pos != std::string_view::npos)
            {
                return line.substr(comment_pos, line.length() - comment_pos);
            }
            else
            {
                return "";
            }
        }();

        new_inputs.push_back(
            make_any_input(type, name, dirty_flag, input_factory, key_values)
        );
    }
}

static auto get_inputs_from_shader_code(std::string_view source_code, DirtyFlag dirty_flag, InputFactory_Ref input_factory)
    -> std::vector<AnyInput>
{
    std::vector<AnyInput> new_inputs;
    std::stringstream     stream{std::string{source_code}};
    std::string           line;
    bool                  has_begun = false;
    while (getline(stream, line))
    {
        if (has_begun)
        {
            if (line == "// END DYNAMIC PARAMS")
            {
                break;
            }
            try
            {
                separate_input_elements(line, dirty_flag, input_factory, new_inputs);
            }
            catch (const std::exception& e)
            {
                Cool::Log::ToUser::error("ShaderManager_FromText::parse_shader_for_params", "Error while parsing :\n{}", e.what());
            }
        }
        if (line == "// BEGIN DYNAMIC PARAMS")
        {
            has_begun = true;
        }
    }
    return new_inputs;
}

void Module_CustomShader::parse_shader_for_params(
    std::string_view    source_code,
    InputFactory_Ref    input_factory,
    InputDestructor_Ref input_destructor
)
{
    auto new_inputs = get_inputs_from_shader_code(source_code, dirty_flag(), input_factory);
    keep_values_of_inputs_that_already_existed_and_destroy_unused_ones(_inputs, new_inputs, input_destructor);
    _inputs = std::move(new_inputs);
}

void Module_CustomShader::set_image_in_shader(std::string_view name, int slot, GLuint texture_id)
{
    if (_shader.pipeline().shader().has_value())
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

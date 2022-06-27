#include "Module_CustomShader.h"
#include <Cool/Camera/CameraShaderU.h>
#include <Cool/Log/ToUser.h>
#include <Cool/String/String.h>
#include <glpp/glpp.hpp>
#include <ranges>
#include <sstream>

namespace Lab {

Module_CustomShader::Module_CustomShader(DirtyFlagFactory_Ref dirty_flag_factory, InputFactory_Ref input_factory)
    : Module{"Custom Shader", dirty_flag_factory}
    , _camera_input{input_factory.make<Cool::Camera>(dirty_flag(), "Camera")}
    , _shader_is_dirty{dirty_flag_factory.make()}
    , _file{_shader_is_dirty}
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
void set_uniform(const Cool::OpenGL::Shader& shader, std::string_view name, const Cool::Color& value)
{
    shader.set_uniform(name, value.rgb);
}

void set_uniform(const Cool::OpenGL::Shader&, std::string_view, const Cool::Camera&)
{
    assert(false); // This isn't used at the moment because we set the camera3d manually for all shaders, but this should be changed
    // Cool::CameraShaderU::set_uniform(shader, value);
}

void Module_CustomShader::render(RenderParams in)
{
    refresh_pipeline_if_necessary(in.provider, in.is_dirty, in.set_clean, in.input_factory, in.input_destructor);
    if (_fullscreen_pipeline.shader())
    {
        _fullscreen_pipeline.shader()->bind();
        _fullscreen_pipeline.shader()->set_uniform("_aspect_ratio", in.provider(Input_AspectRatio{}));
        _fullscreen_pipeline.shader()->set_uniform("_time", in.provider(Input_Time{}));

        for (auto& input : _inputs)
        {
            std::visit([&](auto&& input) {
                set_uniform(*_fullscreen_pipeline.shader(), input.name(), in.provider(input));
            },
                       input);
        }
        Cool::CameraShaderU::set_uniform(*_fullscreen_pipeline.shader(), in.provider(_camera_input), in.provider(Input_AspectRatio{}));
        _fullscreen_pipeline.draw();
    }
}

void Module_CustomShader::refresh_pipeline_if_necessary(
    InputProvider_Ref   provider,
    IsDirty_Ref         is_dirty,
    SetClean_Ref        set_clean,
    InputFactory_Ref    input_factory,
    InputDestructor_Ref input_destructor
)
{
    if (is_dirty(_shader_is_dirty))
    {
        // Cool::Log::ToUser::info("Custom Shader Pipeline", "Re-building pipeline");
        const auto file_path   = provider(_file);
        const auto source_code = Cool::File::to_string(file_path.string());
        compile_shader(source_code, file_path.string());
        parse_shader_for_params(source_code, input_factory, input_destructor);
        set_clean(_shader_is_dirty);
    }
}

void Module_CustomShader::compile_shader(std::string_view fragment_shader_source_code, std::string_view shader_name)
{
    _fullscreen_pipeline.compile(fragment_shader_source_code, shader_name);
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

void identify_type(const std::string type, const std::string name, DirtyFlag dirty_flag, InputFactory_Ref input_factory, std::vector<AnyInput>& new_inputs)
{
    const auto input = [&]() -> AnyInput {
        if (type == "int")
            return input_factory.make<int>(dirty_flag, name);
        else if (type == "float")
            return input_factory.make<float>(dirty_flag, name);
        else if (type == "vec2")
            return input_factory.make<glm::vec2>(dirty_flag, name);
        else if (type == "vec3")
            return input_factory.make<Cool::Color>(dirty_flag, name);
        else
            throw std::invalid_argument(type + " is not a valid parameter type.");
    }();
    new_inputs.push_back(input);
}

auto separate_input_elements(std::string line, DirtyFlag dirty_flag, InputFactory_Ref input_factory, std::vector<AnyInput>& new_inputs)
{
    const auto uniform_pos = line.find("uniform");
    if (uniform_pos != std::string::npos)
    {
        const auto comment_pos = line.find("//");
        if (comment_pos < uniform_pos)
        {
            return;
        }
        auto space_before_include = Cool::String::find_next_word(line, size_t(0));
        if (!space_before_include)
        {
            return;
        }
        line                = Cool::String::replace_at(0, space_before_include->first, line, "");
        const auto type_pos = Cool::String::find_next_word(line, size_t(8));
        if (!type_pos)
        {
            return;
        }
        const std::string type     = line.substr(type_pos->first, type_pos->second - type_pos->first);
        const auto        name_pos = Cool::String::find_next_word(line, type_pos->second);
        if (!name_pos)
        {
            return;
        }
        std::string name = line.substr(name_pos->first, name_pos->second - name_pos->first - 1);

        const std::vector<std::pair<std::string, std::string>> replacements = {
            std::make_pair("_", " "),
        };

        // Cool::String::replace_all(name, "_", " ");

        identify_type(type, name, dirty_flag, input_factory, new_inputs);
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
    if (_fullscreen_pipeline.shader().has_value())
    {
        _fullscreen_pipeline.shader()->bind();
        glpp::active_texture(slot);
        glpp::bind_texture<glpp::TextureKind::Tex2D>(texture_id);
        glpp::set_minification_filter<glpp::TextureKind::Tex2D>(texture_id, glpp::Interpolation::LinearMipmapLinear); // TODO do this only once at texture creation, and only if the curent custom shader needs it
        glpp::generate_mipmap<glpp::TextureKind::Tex2D>();                                                            // TODO DO this only when the texture changes (aka the corresponsing module gets re-rendered)
        _fullscreen_pipeline.shader()->set_uniform(name, slot);
    }
}

} // namespace Lab

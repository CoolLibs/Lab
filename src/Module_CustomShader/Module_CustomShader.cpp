#include "Module_CustomShader.h"
#include <Cool/Camera/CameraShaderU.h>
#include <Cool/InputParser/InputParser.h>
#include <Cool/Log/ToUser.h>
#include <Cool/String/String.h>
#include <doctest/doctest.h>
#include <glpp/glpp.hpp>
#include <ranges>
#include <sstream>
#include <type_from_string/type_from_string.hpp>

// TODO(LD) Support angle units (turns, degrees, radians)

namespace Lab {

Module_CustomShader::Module_CustomShader(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::InputFactory_Ref input_factory)
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
        _shader.pipeline().shader()->set_uniform("_aspect_ratio", in.provider(Cool::Input_AspectRatio{}));
        _shader.pipeline().shader()->set_uniform("_time", in.provider(Cool::Input_Time{}));

        for (auto& input : _inputs)
        {
            std::visit([&](auto&& input) {
                set_uniform(*_shader.pipeline().shader(), input.name(), in.provider(input));
            },
                       input);
        }
        Cool::CameraShaderU::set_uniform(*_shader.pipeline().shader(), in.provider(_camera_input), in.provider(Cool::Input_AspectRatio{}));
        _shader.pipeline().draw();
    }
}

void Module_CustomShader::refresh_pipeline_if_necessary(
    Cool::InputProvider_Ref   provider,
    Cool::IsDirty_Ref         is_dirty,
    Cool::InputFactory_Ref    input_factory,
    Cool::InputDestructor_Ref input_destructor,
    UpdateContext_Ref         update_ctx
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

static auto name(const Cool::AnyInput& input)
{
    return std::visit(([](auto&& input) { return input.name(); }), input);
}

static auto inputs_have_the_same_type_and_name(const Cool::AnyInput& input1, const Cool::AnyInput& input2) -> bool
{
    return input1.index() == input2.index() &&
           name(input1) == name(input2);
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
            *it = std::move(input);
        }
        else
        {
            destroy(input);
        }
    }
}

void Module_CustomShader::parse_shader_for_params(
    std::string_view          source_code,
    Cool::InputFactory_Ref    input_factory,
    Cool::InputDestructor_Ref input_destructor
)
{
    auto new_inputs = Cool::parse_all_inputs(source_code, dirty_flag(), input_factory);
    keep_values_of_inputs_that_already_existed_and_destroy_unused_ones(_inputs, new_inputs, input_destructor);
    _inputs = std::move(new_inputs);
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
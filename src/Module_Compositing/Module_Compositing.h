#pragma once
#include <Cool/Log/MessageSender.h>
#include <Cool/Nodes/NodesGraph.h>
#include <Module/ShaderBased/shader_dependency.h>
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Gpu/FullscreenPipeline.h"
#include "Module/Module.h"

namespace Lab {

class Module_Compositing : public Module {
public:
    Module_Compositing() = default;
    explicit Module_Compositing(Cool::DirtyFlagFactory_Ref);
    Module_Compositing(Module_Compositing const&)                    = delete;
    auto operator=(Module_Compositing const&) -> Module_Compositing& = delete;
    // Module_Compositing(Module_Compositing&&) noexcept                    = default; // TODO(Modules)
    // auto operator=(Module_Compositing&&) noexcept -> Module_Compositing& = default; // TODO(Modules)

    Cool::NodesGraph const*          _nodes_graph; // TODO(Modules) Remove
    Cool::Input<Cool::Camera> const* _camera_input;

    void update(UpdateContext_Ref) override;
    void imgui_windows(Ui_Ref, UpdateContext_Ref) const override;
    void imgui_show_generated_shader_code(Ui_Ref ui);
    auto needs_to_rerender(Cool::IsDirty_Ref) const -> bool override;

    void reset_shader();
    void on_time_reset();

    void set_render_target_size(img::Size const& size);
    void set_shader_code(tl::expected<std::string, std::string> const& shader_code);

    [[nodiscard]] auto depends_on_time() const -> bool { return _dependencies.depends_on_time; }
    [[nodiscard]] auto depends_on_audio() const -> bool { return _dependencies.depends_on_audio(); }

    auto shader_is_valid() const -> bool { return _pipeline.shader().has_value(); } // TODO(Modules) Remove
    auto shader() -> auto const& { return *_pipeline.shader(); }                    // TODO(Modules) Remove
    auto feedback_double_buffer() const -> Cool::DoubleBufferedRenderTarget const& { return _feedback_double_buffer; }

private:
    void render(RenderParams) override;
    void render_impl(RenderParams);
    void log_shader_error(Cool::OptionalErrorMessage const&) const;

private:
    mutable std::string                 _shader_code{};
    mutable Cool::FullscreenPipeline    _pipeline{};
    mutable Cool::MessageSender         _shader_error_sender{};
    Cool::DoubleBufferedRenderTarget    _feedback_double_buffer{};
    mutable ModuleShaderDependencyFlags _dependencies{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Base Module", cereal::base_class<Module>(this))
        );
    }
};

} // namespace Lab

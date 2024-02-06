#pragma once
#include <Cool/Log/MessageSender.h>
#include <Cool/Nodes/NodesGraph.h>
#include <Module/ModuleDependencies.h>
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Gpu/FullscreenPipeline.h"
#include "Module/Module.h"

namespace Lab {

class Module_Compositing : public Module {
public:
    Module_Compositing();
    Module_Compositing(Module_Compositing const&)                    = delete;
    auto operator=(Module_Compositing const&) -> Module_Compositing& = delete;
    // Module_Compositing(Module_Compositing&&) noexcept                    = default; // TODO(Modules)
    // auto operator=(Module_Compositing&&) noexcept -> Module_Compositing& = default; // TODO(Modules)

    Cool::NodesGraph const* _nodes_graph; // TODO(Modules) Remove

    void update() override;
    void imgui_windows(Ui_Ref) const override;
    void imgui_show_generated_shader_code();

    void reset_shader();
    void on_time_reset();

    void set_render_target_size(img::Size const& size);
    void set_shader_code(tl::expected<std::string, std::string> const& shader_code);

    [[nodiscard]] auto depends_on() const -> ModuleDependencies const& { return _depends_on; }
    void               update_dependencies_from_nodes_graph(Cool::NodesGraph const& graph) { Lab::update_dependencies_from_nodes_graph(_depends_on, graph); }

    auto shader_is_valid() const -> bool { return _pipeline.shader().has_value(); } // TODO(Modules) Remove
    auto shader() -> auto const& { return *_pipeline.shader(); }                    // TODO(Modules) Remove
    auto feedback_double_buffer() const -> Cool::DoubleBufferedRenderTarget const& { return _feedback_double_buffer; }

private:
    void render(SystemValues const&) override;
    void render_impl(SystemValues const&);
    void log_shader_error(Cool::OptionalErrorMessage const&) const;

private:
    mutable std::string              _shader_code{};
    mutable Cool::FullscreenPipeline _pipeline{};
    mutable Cool::MessageSender      _shader_error_sender{};
    Cool::DoubleBufferedRenderTarget _feedback_double_buffer{};
    mutable ModuleDependencies       _depends_on{};

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

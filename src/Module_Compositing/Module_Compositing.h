#pragma once
#include <Cool/Log/MessageSender.h>
#include <Cool/Nodes/Editor.h>
#include <Nodes/shader_dependency.h>
#include <memory>
#include <tl/expected.hpp>
#include "Common/FullscreenShader.h"
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Gpu/RenderTarget.h"
#include "Cool/Gpu/Texture.h"
#include "Cool/Nodes/NodeId.h"
#include "Cool/Nodes/NodesFolderWatcher.h"
#include "Cool/Nodes/NodesGraph.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Cool/Path/Path.h"
#include "Module/Module.h"

namespace Lab {

class Module_Compositing : public Module {
public:
    Module_Compositing() = default;
    explicit Module_Compositing(Cool::DirtyFlagFactory_Ref, Cool::InputFactory_Ref);
    Module_Compositing(Module_Compositing const&)                        = default;
    auto operator=(Module_Compositing const&) -> Module_Compositing&     = default;
    Module_Compositing(Module_Compositing&&) noexcept                    = default;
    auto operator=(Module_Compositing&&) noexcept -> Module_Compositing& = default;

    Cool::NodesGraph const* _nodes_graph; // TODO(Modules) Remove

    auto shader_is_valid() const -> bool { return _shader.pipeline().shader().has_value(); } // TODO(Modules) Remove
    auto shader() -> auto const& { return *_shader.pipeline().shader(); }                    // TODO(Modules) Remove

    void update(UpdateContext_Ref) override;
    void imgui_windows(Ui_Ref, UpdateContext_Ref) const override;
    auto is_dirty(Cool::IsDirty_Ref) const -> bool override;

    auto shader_dirty_flag() const -> Cool::DirtyFlag const& { return _shader.dirty_flag(); }

    void reset();

    void set_shader_code(tl::expected<std::string, std::string> const& shader_code, UpdateContext_Ref update_ctx);

    [[nodiscard]] auto depends_on_time() const -> bool { return _dependencies.depends_on_time; }
    [[nodiscard]] auto depends_on_audio() const -> bool { return _dependencies.depends_on_audio_volume || _dependencies.depends_on_audio_waveform || _dependencies.depends_on_audio_spectrum; }

    auto feedback_double_buffer() const -> Cool::DoubleBufferedRenderTarget const& { return _feedback_double_buffer; }

private:
    void render(RenderParams, UpdateContext_Ref) override;
    void render_impl(RenderParams, UpdateContext_Ref);
    void compute_dependencies(); // We don't want to rerender when audio / time changes if we don't depend on them. Also, audio features are costly to compute, so we only set these uniforms in the shader if we actually need them.

private:
    mutable std::string              _shader_code{};
    mutable FullscreenShader         _shader{};
    mutable Cool::MessageSender      _shader_compilation_error{};
    Cool::DoubleBufferedRenderTarget _feedback_double_buffer{};

    ModuleShaderDependencyFlags _dependencies;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Base Module", cereal::base_class<Module>(this)),
            cereal::make_nvp("Shader", _shader)
        );
    }
};

} // namespace Lab

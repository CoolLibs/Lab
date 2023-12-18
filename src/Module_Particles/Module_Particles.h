#pragma once
#include <cstddef>
#include <optional>
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Nodes/NodeId.h"
#include "Cool/Nodes/NodesGraph.h"
#include "Cool/Particles/ParticleSystem.h"
#include "Module/Module.h"
#include "Nodes/shader_dependency.h"

namespace Lab {
class Module_Particles : public Module {
public:
    Module_Particles();
    explicit Module_Particles(Cool::DirtyFlagFactory_Ref, Cool::InputFactory_Ref);
    Module_Particles(Module_Particles const&)                        = delete;
    auto operator=(Module_Particles const&) -> Module_Particles&     = delete;
    Module_Particles(Module_Particles&&) noexcept                    = default;
    auto operator=(Module_Particles&&) noexcept -> Module_Particles& = default;
    ~Module_Particles() override                                     = default;

    Cool::NodesGraph const*                 _nodes_graph;            // TODO(Particles) Remove
    Cool::DoubleBufferedRenderTarget const* _feedback_double_buffer; // TODO(Particles) Remove
    Cool::Input<Cool::Camera> const*        _camera_input;

    void update(UpdateContext_Ref) override;
    void update_particles(UpdateContext_Ref);
    void imgui_windows(Ui_Ref, UpdateContext_Ref) const override;
    auto needs_to_rerender(Cool::IsDirty_Ref) const -> bool override;

    void imgui_debug_menu(Cool::SetDirty_Ref set_dirty) const;

    void set_simulation_shader_code(tl::expected<std::string, std::string> const& shader_code, UpdateContext_Ref update_ctx, bool for_testing_nodes, size_t dimension);
    void on_time_reset();

    [[nodiscard]] auto depends_on_time() const -> bool { return _dependencies.depends_on_time; }
    [[nodiscard]] auto depends_on_audio() const -> bool { return _dependencies.depends_on_audio_volume || _dependencies.depends_on_audio_waveform || _dependencies.depends_on_audio_spectrum; }

private:
    void render(RenderParams, UpdateContext_Ref) override;
    auto create_particle_system() const -> std::optional<Cool::ParticleSystem>;
    void compute_dependencies();
    // void recreate_particle_system(); // TODO(Modules) Remove me, this is for tests only

private:
    mutable std::string         _shader_code{};
    mutable Cool::MessageSender _shader_compilation_error{};

    ModuleShaderDependencyFlags _dependencies;

    mutable size_t                              _particles_count{5'000}; // TODO(History) Change through command
    mutable float                               _particle_size{0.01f};   // TODO(History) Change through command
    mutable std::optional<Cool::ParticleSystem> _particle_system;

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

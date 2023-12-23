#pragma once
#include <cstddef>
#include <optional>
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Nodes/NodeId.h"
#include "Cool/Nodes/NodesGraph.h"
#include "Cool/Particles/ParticleSystem.h"
#include "Module/Module.h"
#include "Module/ShaderBased/shader_dependency.h"

namespace Lab {
class Module_Particles : public Module {
public:
    Module_Particles();
    explicit Module_Particles(Cool::DirtyFlagFactory_Ref, Cool::NodeId);
    Module_Particles(Module_Particles const&)                        = delete;
    auto operator=(Module_Particles const&) -> Module_Particles&     = delete;
    Module_Particles(Module_Particles&&) noexcept                    = default;
    auto operator=(Module_Particles&&) noexcept -> Module_Particles& = default;
    ~Module_Particles() override                                     = default;

    Cool::NodesGraph const*                 _nodes_graph{};          // TODO(Particles) Remove
    Cool::DoubleBufferedRenderTarget const* _feedback_double_buffer; // TODO(Particles) Remove
    Cool::Input<Cool::Camera> const*        _camera_input;

    void update(UpdateContext_Ref) override;
    void request_particles_to_update() { _needs_to_update_particles = true; }
    void imgui_windows(Ui_Ref, UpdateContext_Ref) const override;
    void imgui_show_generated_shader_code() const;
    auto needs_to_rerender(Cool::IsDirty_Ref) const -> bool override;

    void set_simulation_shader_code(tl::expected<std::string, std::string> const& shader_code, bool for_testing_nodes, size_t dimension);
    void on_time_reset();

    [[nodiscard]] auto depends_on_time() const -> bool { return _dependencies.depends_on_time; }
    [[nodiscard]] auto depends_on_audio() const -> bool { return _dependencies.depends_on_audio_volume || _dependencies.depends_on_audio_waveform || _dependencies.depends_on_audio_spectrum; }

private:
    void render(RenderParams) override;
    void update_particles(Cool::InputProvider_Ref);
    auto create_particle_system() const -> std::optional<Cool::ParticleSystem>;
    void compute_dependencies();
    void update_particles_count_ifn(UpdateContext_Ref);
    auto desired_particles_count() const -> size_t;

private:
    mutable std::string         _shader_code{};
    mutable Cool::MessageSender _shader_compilation_error{};
    bool                        _needs_to_update_particles{true};

    ModuleShaderDependencyFlags _dependencies; // TODO(Particles) Two dependencies, one for each shader

    Cool::NodeId                                _initializer_id{};
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

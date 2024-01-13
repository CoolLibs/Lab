#pragma once
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Nodes/NodeId.h"
#include "Cool/Nodes/NodesGraph.h"
#include "Cool/Particles/ParticleSystem.h"
#include "Module/Module.h"
#include "Module/ModuleDependencies.h"

namespace Lab {
class Module_Particles : public Module {
public:
    Module_Particles() = default;
    explicit Module_Particles(Cool::DirtyFlagFactory_Ref, Cool::NodeId const& initializer_node_id);
    Module_Particles(Module_Particles const&)                        = delete;
    auto operator=(Module_Particles const&) -> Module_Particles&     = delete;
    Module_Particles(Module_Particles&&) noexcept                    = default;
    auto operator=(Module_Particles&&) noexcept -> Module_Particles& = default;
    ~Module_Particles() override                                     = default;

    Cool::NodesGraph const*                 _nodes_graph{};            // TODO(Particles) Remove
    Cool::DoubleBufferedRenderTarget const* _feedback_double_buffer{}; // TODO(Particles) Remove
    Cool::Input<Cool::Camera> const*        _camera_input{};           // TODO(Particles) Remove

    void update(UpdateContext_Ref) override;
    void request_particles_to_update() { _needs_to_update_particles = true; }
    void imgui_windows(Ui_Ref, UpdateContext_Ref) const override;
    void imgui_show_generated_shader_code();

    void set_simulation_shader_code(tl::expected<std::string, std::string> const& shader_code, bool for_testing_nodes, int dimension);
    void on_time_reset();

    [[nodiscard]] auto depends_on_time() const -> bool { return _depends_on.time; }
    [[nodiscard]] auto depends_on_audio() const -> bool { return _depends_on.audio(); }

private:
    void render(RenderParams) override;
    void update_particles(Cool::InputProvider_Ref);
    auto create_particle_system() const -> std::optional<Cool::ParticleSystem>;
    void update_particles_count_ifn(UpdateContext_Ref);
    auto desired_particles_count() const -> size_t;
    void log_simulation_shader_error(Cool::OptionalErrorMessage const&) const;

private:
    mutable std::optional<Cool::ParticleSystem> _particle_system;
    ModuleDependencies                          _depends_on{}; // TODO(Particles) Two dependencies, one for each shader (simulation and render)
    Cool::NodeId                                _initializer_node_id{};
    bool                                        _needs_to_update_particles{true};
    mutable Cool::MessageSender                 _simulation_shader_error_sender{};
    mutable std::string                         _shader_code{};

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

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
    Module_Particles() = default; // TODO(FeedbackLoop) remove?
    explicit Module_Particles(Cool::NodeId const& id_of_node_storing_particles_count);
    Module_Particles(Module_Particles const&)                        = delete;
    auto operator=(Module_Particles const&) -> Module_Particles&     = delete;
    Module_Particles(Module_Particles&&) noexcept                    = default;
    auto operator=(Module_Particles&&) noexcept -> Module_Particles& = default;
    ~Module_Particles() override                                     = default;

    void update() override;
    void on_time_changed() override;
    void imgui_show_generated_shader_code();

    [[nodiscard]] auto needs_to_rerender() const -> bool override
    {
        return Module::needs_to_rerender() || _needs_to_update_particles;
    };

    void set_simulation_shader_code(tl::expected<std::string, std::string> const& shader_code, bool for_testing_nodes, int dimension);
    void on_time_reset() override;

    [[nodiscard]] auto depends_on() const -> ModuleDependencies const& { return _depends_on; }
    void               update_dependencies_from_nodes_graph(Cool::NodesGraph const& graph) override { Lab::update_dependencies_from_nodes_graph(_depends_on, graph); }

private:
    void render(DataToPassToShader const&) override;
    void update_particles(DataToPassToShader const&);
    auto create_particle_system() const -> std::optional<Cool::ParticleSystem>;
    void update_particles_count_ifn();
    auto desired_particles_count() const -> size_t;
    void log_simulation_shader_error(Cool::OptionalErrorMessage const&) const;
    void request_particles_to_reset();
    void request_particles_to_update() { _needs_to_update_particles = true; }

private:
    mutable std::optional<Cool::ParticleSystem> _particle_system{};
    int                                         _particle_system_dimension{};
    ModuleDependencies                          _depends_on{}; // TODO(Particles) Two dependencies, one for each shader (simulation and render)
    Cool::NodeId                                _id_of_node_storing_particles_count{};
    bool                                        _needs_to_update_particles{true};
    bool                                        _force_init_particles{true};
    mutable Cool::MessageSender                 _simulation_shader_error_sender{};
    mutable std::string                         _shader_code{};

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Base Module", ser20::base_class<Module>(this))
        );
    }
};

} // namespace Lab

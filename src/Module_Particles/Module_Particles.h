#pragma once
#include "Cool/Dependencies/SharedVariable.h"
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Nodes/NodeId.h"
#include "Cool/Nodes/NodesGraph.h"
#include "Cool/Particles/ParticleSystem.h"
#include "Cool/Variables/Variable.h"
#include "Module/Module.h"
#include "Module/ModuleDependencies.h"

namespace Lab {

inline auto delta_time() -> Cool::SharedVariable<float>&
{
    static auto inst = Cool::SharedVariable<float>{Cool::Variable<float>{Cool::VariableData<float>{.name = "Delta Time", .value = 3.f}}, {}};
    return inst;
}
inline auto diffusion_rate_a() -> Cool::SharedVariable<float>&
{
    static auto inst = Cool::SharedVariable<float>{Cool::Variable<float>{Cool::VariableData<float>{.name = "Diff A", .value = 0.3f}}, {}};
    return inst;
}
inline auto diffusion_rate_b() -> Cool::SharedVariable<float>&
{
    static auto inst = Cool::SharedVariable<float>{Cool::Variable<float>{Cool::VariableData<float>{.name = "Diff B", .value = 0.15f}}, {}};
    return inst;
}
inline auto feed_rate_a() -> Cool::SharedVariable<float>&
{
    static auto inst = Cool::SharedVariable<float>{Cool::Variable<float>{Cool::VariableData<float>{.name = "Feed A", .value = 0.0545f}}, {}};
    return inst;
}
inline auto kill_rate_b() -> Cool::SharedVariable<float>&
{
    static auto inst = Cool::SharedVariable<float>{Cool::Variable<float>{Cool::VariableData<float>{.name = "Kill B", .value = 0.062f}}, {}};
    return inst;
}

class Module_Particles : public Module {
public:
    Module_Particles() = default;
    explicit Module_Particles(Cool::NodeId const& id_of_node_storing_particles_count, std::string texture_name_in_shader, std::vector<std::shared_ptr<Module>> modules_that_we_depend_on, std::vector<Cool::NodeId> nodes_that_we_depend_on);

    void update() override;
    void on_time_changed() override;
    void imgui_windows(Ui_Ref) const override;

    [[nodiscard]] auto needs_to_rerender() const -> bool override
    {
        return Module::needs_to_rerender() || _needs_to_update_particles;
    };

    void set_simulation_shader_code(tl::expected<std::string, std::string> const& shader_code, bool for_testing_nodes, int dimension);
    void on_time_reset() override;

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
    // ModuleDependencies                          _depends_on{}; // TODO(Particles) Two dependencies, one for each shader (simulation and render)
    Cool::NodeId                _id_of_node_storing_particles_count{};
    bool                        _needs_to_update_particles{true};
    bool                        _force_init_particles{true};
    mutable Cool::MessageSender _simulation_shader_error_sender{};
    mutable std::string         _shader_code{};

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Base Module", ser20::base_class<Module>(this)),
            ser20::make_nvp("a", delta_time()),
            ser20::make_nvp("b", diffusion_rate_a()),
            ser20::make_nvp("c", diffusion_rate_b()),
            ser20::make_nvp("d", feed_rate_a()),
            ser20::make_nvp("e", kill_rate_b())
        );
    }
};

} // namespace Lab

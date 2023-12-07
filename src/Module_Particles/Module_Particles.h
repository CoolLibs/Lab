#pragma once
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Nodes/NodeId.h"
#include "Cool/Nodes/NodesGraph.h"
#include "Cool/Particles/ParticleSystem.h"
#include "Module/Module.h"

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

    Cool::NodesGraph const* _nodes_graph; // TODO(Particles) Remove
    Cool::NodeId            _main_node_id{};

    void update(UpdateContext_Ref) override;
    void update_particles(UpdateContext_Ref);
    void imgui_windows(Ui_Ref, UpdateContext_Ref) const override;
    auto is_dirty(Cool::IsDirty_Ref) const -> bool override;

    void imgui_debug_menu(Cool::SetDirty_Ref set_dirty);

    void compile(Cool::NodesGraph const& nodes_graph, Cool::NodeId const& root_node_id, UpdateContext_Ref, bool for_testing_nodes = false);

private:
    void render(RenderParams, UpdateContext_Ref) override;

    auto create_particle_system() const -> std::optional<Cool::ParticleSystem>;
    // void recreate_particle_system(); // TODO(Modules) Remove me, this is for tests only

private:
    mutable std::string         _shader_code{};
    mutable Cool::MessageSender _shader_compilation_error{};

    size_t                              _particles_count{5'000};
    std::optional<Cool::ParticleSystem> _particle_system;

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

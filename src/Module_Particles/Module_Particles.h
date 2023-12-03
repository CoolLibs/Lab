#pragma once
#include "Module/Module.h"
#include "ParticleSystem.h"

namespace Lab {

class Module_Particles : public Module {
public:
    Module_Particles() = default;
    explicit Module_Particles(Cool::DirtyFlagFactory_Ref, Cool::InputFactory_Ref);
    Module_Particles(Module_Particles const&)                        = default;
    auto operator=(Module_Particles const&) -> Module_Particles&     = default;
    Module_Particles(Module_Particles&&) noexcept                    = default;
    auto operator=(Module_Particles&&) noexcept -> Module_Particles& = default;

    void update(UpdateContext_Ref) override;
    void update_particles(UpdateContext_Ref);
    void imgui_windows(Ui_Ref, UpdateContext_Ref) const override;
    auto is_dirty(Cool::IsDirty_Ref) const -> bool override;

    void render() { _particle_system.render(); }
    void imgui_debug_menu() { _particle_system.imgui_debug_menu(); }

private:
    void render(RenderParams, UpdateContext_Ref) override;

private:
    ParticleSystem _particle_system{};

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

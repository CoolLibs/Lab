#pragma once
#include "Module/Module.h"
#include "ParticleSystem.h"

namespace Lab {

class Module_Particles : public Module {
public:
    Module_Particles() = default;
    explicit Module_Particles(Cool::DirtyFlagFactory_Ref, Cool::InputFactory_Ref);

    void update(UpdateContext_Ref) override;
    void imgui_windows(Ui_Ref, UpdateContext_Ref) const override;
    void submit_gizmos(Cool::GizmoManager&, UpdateContext_Ref) override;
    auto all_inputs() const -> Cool::AllInputRefsToConst override;
    auto is_dirty(Cool::IsDirty_Ref) const -> bool override;

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

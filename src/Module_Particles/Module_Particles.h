#pragma once
#include <optional>
#include "Common/FullscreenShader.h"
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
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

    Cool::NodesGraph const*                 _nodes_graph;            // TODO(Particles) Remove
    Cool::DoubleBufferedRenderTarget const* _feedback_double_buffer; // TODO(Particles) Remove

    void update(UpdateContext_Ref) override;
    void update_particles(UpdateContext_Ref);
    void imgui_windows(Ui_Ref, UpdateContext_Ref) const override;
    auto is_dirty(Cool::IsDirty_Ref) const -> bool override;

    void imgui_debug_menu(Cool::SetDirty_Ref set_dirty);

    void set_simulation_shader_code(tl::expected<std::string, std::string> const& shader_code, UpdateContext_Ref update_ctx, bool for_testing_nodes);

    [[nodiscard]] auto depends_on_time() const -> bool { return _depends_on_time; }
    [[nodiscard]] auto depends_on_particles() const -> bool { return _depends_on_particles; }
    [[nodiscard]] auto depends_on_audio() const -> bool { return _depends_on_audio_volume || _depends_on_audio_waveform || _depends_on_audio_spectrum; }

private:
    void render(RenderParams, UpdateContext_Ref) override;
    auto create_particle_system() const -> std::optional<Cool::ParticleSystem>;
    void compute_dependencies();
    // void recreate_particle_system(); // TODO(Modules) Remove me, this is for tests only

private:
    mutable std::string         _shader_code{};
    mutable Cool::MessageSender _shader_compilation_error{};

    bool _depends_on_time{false};
    bool _depends_on_particles{false};
    bool _depends_on_audio_volume{false};
    bool _depends_on_audio_waveform{false};
    bool _depends_on_audio_spectrum{false};

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

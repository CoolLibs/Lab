#pragma once
#include <Dependencies/Camera2DManager.h>
#include "Cool/Audio/AudioManager.h"
#include "Cool/Exporter/Exporter.h"
#include "Cool/Image/ImageSizeConstraint.h"
#include "Cool/Midi/MidiManager.h"
#include "Cool/OSC/OSCConnectionEndpoint.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "Cool/Time/Clock_Realtime.h"
#include "Dependencies/Camera3DManager.h"
#include "Dump/coollab_version.h"
#include "ModulesGraph/ModulesGraph.h"

namespace Lab {

struct Project {
    Project();

    std::unique_ptr<ModulesGraph> modules_graph{std::make_unique<ModulesGraph>()}; // TODO(Modules) Can't we avoid the unique_ptr?
    Camera3DManager               camera_3D_manager;                               // Must be after modules_graph because we reference a DirtyFlag from it
    Camera2DManager               camera_2D_manager;                               // Must be after modules_graph because we reference a DirtyFlag from it
    Cool::Clock_Realtime          clock;
    Cool::ImageSizeConstraint     view_constraint;
    History                       history{};
    Cool::Exporter                exporter;
    Cool::AudioManager            audio;
    Cool::OSCConnectionEndpoint   osc_endpoint{};

    std::string debug_info_coollab_version{}; // Only used to generate an error message when deserialization fails.

    [[nodiscard]] auto is_empty() const -> bool;
    [[nodiscard]] auto current_clock() const -> Cool::Clock const& { return exporter.is_exporting() ? exporter.clock() : clock; }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        debug_info_coollab_version = coollab_version();
#if defined(__linux__)
        history.set_max_saved_size(0); // TODO HACK to avoid a crash when deserializing the history: https://github.com/orgs/CoolLibs/projects/1/views/1?pane=issue&itemId=46983814
#endif
        archive(
            cereal::make_nvp("Coollab version", debug_info_coollab_version), // Must be first, purely informative, so that users can know what version of Coollab a project was built with.
            cereal::make_nvp("Time", clock),
            cereal::make_nvp("View Constraint", view_constraint),
            cereal::make_nvp("Exporter (Image and Video)", exporter),
            cereal::make_nvp("Camera 3D Manager", camera_3D_manager),
            cereal::make_nvp("Camera 2D Manager", camera_2D_manager),
            cereal::make_nvp("Modules Graph", modules_graph),
            cereal::make_nvp("History", history),
            cereal::make_nvp("Audio", audio),
            cereal::make_nvp("OSC Endpoint", osc_endpoint),
            cereal::make_nvp("MIDI Channels", Cool::midi_manager().all_values())
        );
    }
};

} // namespace Lab
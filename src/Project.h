#pragma once
#include <Dependencies/Camera2DManager.h>
#include <reg/src/AnyId.hpp>
#include <reg/src/internal/generate_uuid.hpp>
#include "Cool/Audio/AudioManager.h"
#include "Cool/Exporter/Exporter.h"
#include "Cool/Image/ImageSizeConstraint.h"
#include "Cool/Image/SharedAspectRatio.hpp"
#include "Cool/Midi/MidiManager.h"
#include "Cool/OSC/OSCConnectionEndpoint.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "Cool/Time/Clock_Realtime.h"
#include "Dependencies/Camera3DManager.h"
#include "ModulesGraph/ModulesGraph.h"

namespace Lab {

struct Project {
    Project();
    reg::AnyId uuid{reg::internal::generate_uuid()};

    std::unique_ptr<ModulesGraph> modules_graph{std::make_unique<ModulesGraph>()}; // TODO(Modules) Can't we avoid the unique_ptr?
    Camera3DManager               camera_3D_manager;                               // Must be after modules_graph because we reference a DirtyFlag from it
    Camera2DManager               camera_2D_manager;                               // Must be after modules_graph because we reference a DirtyFlag from it
    Cool::Clock_Realtime          clock;
    Cool::ImageSizeConstraint     view_constraint;
    History                       history{};
    Cool::Exporter                exporter;
    Cool::AudioManager            audio;
    Cool::OSCConnectionEndpoint   osc_endpoint{};
    Cool::SharedAspectRatio       shared_aspect_ratio{};

    [[nodiscard]] auto is_empty() const -> bool;
    [[nodiscard]] auto current_clock() const -> Cool::Clock const& { return exporter.is_exporting() ? exporter.clock() : clock; }

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
#if defined(__linux__)
        history.set_max_saved_size(0); // TODO HACK to avoid a crash when deserializing the history: https://github.com/orgs/CoolLibs/projects/1/views/1?pane=issue&itemId=46983814
#endif
        archive(
            ser20::make_nvp("UUID", uuid),
            ser20::make_nvp("Time", clock),
            ser20::make_nvp("View Constraint", view_constraint),
            ser20::make_nvp("Exporter (Image and Video)", exporter),
            ser20::make_nvp("Camera 3D Manager", camera_3D_manager),
            ser20::make_nvp("Camera 2D Manager", camera_2D_manager),
            ser20::make_nvp("Modules Graph", modules_graph),
            ser20::make_nvp("History", history),
            ser20::make_nvp("Audio", audio),
            ser20::make_nvp("OSC Endpoint", osc_endpoint),
            ser20::make_nvp("MIDI Channels", Cool::midi_manager().all_values()),
            ser20::make_nvp("Shared Aspect Ratio", shared_aspect_ratio)
        );
    }
};

} // namespace Lab
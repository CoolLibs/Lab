#pragma once
#include "Cool/Audio/AudioManager.h"
#include "Cool/Camera/CameraManager.h"
#include "Cool/Dependencies/VariableRegistries.h"
#include "Cool/Exporter/Exporter.h"
#include "Cool/Image/ImageSizeConstraint.h"
#include "Cool/OSC/OSCConnectionEndpoint.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "Cool/Time/Clock_Realtime.h"
#include "Dependencies/CameraManager.h"
#include "Dump/coollab_version.h"
#include "ModulesGraph/ModulesGraph.h"

namespace Lab {

struct Project {
    Project();

    Cool::VariableRegistries       variable_registries; // First because modules need the registries when they get created
    CameraManager                  camera_manager;      // First because modules need the camera id when they get created
    Cool::Variable<Cool::Camera2D> camera2D;
    Cool::Clock_Realtime           clock;
    Cool::ImageSizeConstraint      view_constraint;
    Cool::DirtyRegistry            dirty_registry; // Before the modules because it is used to create them
    History                        history{};
    std::unique_ptr<ModulesGraph>  modules_graph; // TODO(Modules) Can't we avoid the unique_ptr?
    bool                           is_first_frame{true};
    bool                           is_camera_2D_editable_in_view{true};
    Cool::Exporter                 exporter;
    Cool::AudioManager             audio;
    Cool::OSCConnectionEndpoint    osc_endpoint{};

    std::string debug_info_coollab_version{}; // Only used to generate an error message when deserialization fails.

    [[nodiscard]] auto is_empty() const -> bool;

    auto input_factory() { return Cool::InputFactory_Ref{variable_registries, camera_manager.id()}; }
    auto dirty_flag_factory() { return Cool::DirtyFlagFactory_Ref{dirty_registry}; }

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
            cereal::make_nvp("Camera Manager", camera_manager),
            cereal::make_nvp("Camera 2D", camera2D),
            cereal::make_nvp("Is camera 2D editable in view", is_camera_2D_editable_in_view),
            cereal::make_nvp("Modules Graph", modules_graph),
            cereal::make_nvp("Variable Registries", variable_registries),
            cereal::make_nvp("History", history),
            cereal::make_nvp("Dirty Registry", dirty_registry),
            cereal::make_nvp("Audio", audio),
            cereal::make_nvp("OSC Endpoint", osc_endpoint)
        );
    }
};

} // namespace Lab
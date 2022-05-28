#pragma once

#include <Cool/Camera/CameraManager.h>
#include <reg/reg.hpp>

namespace Lab {

class CommandDispatcher;

class CameraManager {
public:
    explicit CameraManager(const reg::Id<Cool::Camera>& id)
        : _id{id}
    {
    }

    CameraManager() = default;

    void update(const CommandDispatcher&) const;

    void set_camera(const Cool::Camera&);

    auto manager() -> Cool::CameraManager& { return _camera_manager; }

    auto id() const -> const reg::Id<Cool::Camera>& { return _id; }

private:
    Cool::CameraManager   _camera_manager;
    Cool::Camera          _previous_camera;
    reg::Id<Cool::Camera> _id;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("CameraManager", _camera_manager),
                cereal::make_nvp("ID", _id));
    }
};

} // namespace Lab
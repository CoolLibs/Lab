#pragma once

#include <Cool/Dependencies/Dirty.h>
#include <Cool/Dependencies/SetVariableDirty_Ref.h>
#include <Cool/Dependencies/VariableId.h>
#include <Cool/Dependencies/VariableRegistries.h>
#include <utility>
#include "Cool/Window/Window.h"
#include "Dependencies/History.h"

namespace Lab {

class CameraManager;
class App;
struct Project;

// This is a class rather than a struct because we want to use methods to access the different members
// this will make it easier to track down who is using what
// and since this class is a big thing that makes everything accessible to everyone, it is especially important to be able to track the use that is made of each member.
class CommandExecutionContext_Ref {
public:
    [[nodiscard]] auto app() const -> App& { return _data.app; }
    [[nodiscard]] auto history() const -> const History& { return _data.history; }
    [[nodiscard]] auto registries() const -> Cool::VariableRegistries& { return _data.registries; }
    [[nodiscard]] auto camera_manager() const -> CameraManager& { return _data.camera_manager; }
    [[nodiscard]] auto main_window() const -> Cool::Window& { return _data.main_window; }
    [[nodiscard]] auto project() const -> Project& { return _data.project; }
    [[nodiscard]] auto project_path() const -> std::optional<std::filesystem::path>& { return _data.project_path; }
    template<typename T>
    void set_dirty(const Cool::VariableId<T>& id)
    {
        _data.set_dirty(id);
    }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        std::reference_wrapper<App>                                  app;
        std::reference_wrapper<const History>                        history;
        std::reference_wrapper<Cool::VariableRegistries>             registries;
        std::reference_wrapper<CameraManager>                        camera_manager;
        Cool::SetVariableDirty_Ref                                   set_dirty;
        std::reference_wrapper<Cool::Window>                         main_window;
        std::reference_wrapper<Project>                              project;
        std::reference_wrapper<std::optional<std::filesystem::path>> project_path;
    };
    explicit CommandExecutionContext_Ref(Data data)
        : _data{std::move(data)}
    {}

private:
    Data _data;
};

} // namespace Lab
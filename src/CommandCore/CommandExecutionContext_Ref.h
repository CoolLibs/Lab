#pragma once
#include <CommandCore/CommandExecutor_TopLevel.h>
#include "ConcreteCommand.h"
#include "Cool/Window/Window.h"
#include "Dependencies/History.h"

namespace Lab {

class Camera3DManager;
class App;
class RecentlyOpened;
struct Project;
class ModulesGraph;
class CommandExecutor;

// This is a class rather than a struct because we want to use methods to access the different members
// this will make it easier to track down who is using what
// and since this class is a big thing that makes everything accessible to everyone, it is especially important to be able to track the use that is made of each member.
class CommandExecutionContext_Ref {
public:
    [[nodiscard]] auto app() const -> App& { return _data.app; }
    [[nodiscard]] auto history() const -> History const& { return _data.history; }
    [[nodiscard]] auto camera_manager() const -> Camera3DManager& { return _data.camera_manager; }
    [[nodiscard]] auto main_window() const -> Cool::Window& { return _data.main_window; }
    [[nodiscard]] auto project() const -> Project& { return _data.project; }
    [[nodiscard]] auto project_path() const -> std::optional<std::filesystem::path>& { return _data.project_path; }
    [[nodiscard]] auto command_executor() const -> CommandExecutor;
    [[nodiscard]] auto recently_opened_projects() const -> RecentlyOpened& { return _data.recently_opened; }
    [[nodiscard]] auto modules_graph() const -> ModulesGraph& { return _data.modules_graph; }
    template<ConcreteCommand ConcreteCommandT>
    void execute(ConcreteCommandT const& command) const
    {
        _data.executor.execute(command, *this);
    }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        std::reference_wrapper<App>                                  app;
        std::reference_wrapper<History const>                        history;
        std::reference_wrapper<Camera3DManager>                      camera_manager;
        std::reference_wrapper<Cool::Window>                         main_window;
        std::reference_wrapper<Project>                              project;
        std::reference_wrapper<std::optional<std::filesystem::path>> project_path;
        CommandExecutor_TopLevel                                     executor;
        std::reference_wrapper<RecentlyOpened>                       recently_opened;
        std::reference_wrapper<ModulesGraph>                         modules_graph;
    };
    explicit CommandExecutionContext_Ref(Data data)
        : _data{std::move(data)}
    {}

private:
    Data _data;
};

} // namespace Lab
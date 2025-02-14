#pragma once
#include "Project.hpp"
#include "ProjectManagerImpl.hpp"

namespace Lab {

class ProjectManager {
public:
    auto project() -> Project& { return _impl.project(); }
    auto project() const -> Project const& { return _impl.project(); }
    auto project_path() const -> std::filesystem::path { return _impl.project_path(); }
    auto info_folder_for_the_launcher() const -> std::optional<std::filesystem::path> { return _impl.info_folder_for_the_launcher(); }
    auto has_registered_project_to_the_launcher() const -> bool { return _impl.has_registered_project_to_the_launcher(); }

    void process_command_line_args(OnProjectLoaded const& on_project_loaded, SetWindowTitle const& set_window_title);
    void create_new_project(OnProjectLoaded const& on_project_loaded, SetWindowTitle const& set_window_title);
    void create_new_project_in_folder(std::filesystem::path const& folder_path, OnProjectLoaded const& on_project_loaded, SetWindowTitle const& set_window_title);
    void create_new_project_in_file(std::filesystem::path file_path, OnProjectLoaded const& on_project_loaded, SetWindowTitle const& set_window_title);
    void open_project(std::filesystem::path const& file_path, OnProjectLoaded const& on_project_loaded, SetWindowTitle const& set_window_title);

    void autosave_project(SetWindowTitle const& set_window_title);
    void save_project(SetWindowTitle const& set_window_title);
    void save_project_as(std::filesystem::path file_path, SetWindowTitle const& set_window_title, SaveThumbnail const& save_thumbnail, bool register_project_in_the_launcher = true);
    void package_project_into(std::filesystem::path const& folder_path, SetWindowTitle const& set_window_title, SaveThumbnail const& save_thumbnail, bool register_project_in_the_launcher = true);
    void rename_project(std::string new_name, SetWindowTitle const& set_window_title);

    void imgui_project_name_in_the_middle_of_the_menu_bar(SetWindowTitle const& set_window_title);

    auto file_dialog_to_save_project() -> std::optional<std::filesystem::path>;
    auto force_file_dialog_to_save_project() -> std::filesystem::path;

private:
    internal::ProjectManagerImpl _impl;
};

} // namespace Lab
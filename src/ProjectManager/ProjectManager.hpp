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

    void process_command_line_args(OnProjectLoaded const&, OnProjectUnloaded const&, SetWindowTitle const&);
    void create_new_project(OnProjectLoaded const&, SetWindowTitle const&);
    void create_new_project_in_folder(std::filesystem::path const& folder_path, OnProjectLoaded const&, SetWindowTitle const&);
    void create_new_project_in_file(std::filesystem::path file_path, OnProjectLoaded const&, SetWindowTitle const&);

    // We only open projects at the beginning of a frame, because
    void open_project_on_next_frame(std::filesystem::path const& file_path);
    void open_requested_project_if_any(OnProjectLoaded const&, OnProjectUnloaded const&, SetWindowTitle const&);

    auto autosave_project(bool must_absolutely_succeed, SetWindowTitle const&) -> bool;
    auto save_project(bool must_absolutely_succeed, SetWindowTitle const&) -> bool;
    auto save_project_as(std::filesystem::path file_path, SaveThumbnail const&, bool register_project_in_the_launcher = true) -> bool;
    auto package_project_into(std::filesystem::path const& folder_path, SaveThumbnail const&, bool register_project_in_the_launcher = true) -> bool;
    auto rename_project(std::string new_name, SetWindowTitle const&) -> bool;

    void imgui_project_name_in_the_middle_of_the_menu_bar(SetWindowTitle const&);

    auto file_dialog_to_save_project() -> std::optional<std::filesystem::path>;
    auto file_dialog_to_open_project() -> std::optional<std::filesystem::path>;
    auto force_file_dialog_to_save_project() -> std::filesystem::path;

private:
    /// Returns true iff we saved successfully
    [[nodiscard]] auto save_project_impl(bool must_absolutely_succeed, SetWindowTitle const&) -> bool;
    [[nodiscard]] auto save_project_impl(std::filesystem::path file_path, bool must_absolutely_succeed, SetWindowTitle const&) -> bool;

    void open_project(std::filesystem::path const& file_path, OnProjectLoaded const&, OnProjectUnloaded const&, SetWindowTitle const&);

private:
    internal::ProjectManagerImpl         _impl{};
    std::optional<std::filesystem::path> _project_to_open_on_next_frame{};
};

} // namespace Lab
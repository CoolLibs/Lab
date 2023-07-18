#pragma once

namespace Lab::Path {

auto user_folder() -> std::filesystem::path;
auto backup_project() -> std::filesystem::path;

} // namespace Lab::Path
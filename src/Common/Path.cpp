#include "Path.h"
#include "Cool/Path/Path.h"

namespace Lab::Path {

auto user_folder() -> std::filesystem::path
{
    return Cool::Path::root() / "User Data";
}

auto backup_project() -> std::filesystem::path
{
    return user_folder() / "backup.clb";
}

} // namespace Lab::Path
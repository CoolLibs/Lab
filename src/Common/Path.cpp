#include "Path.h"
#include "Cool/Path/Path.h"
#include "ProjectManager/FileExtension.h"

namespace Lab::Path {

auto untitled_project() -> std::filesystem::path
{
    return Cool::Path::user_data() / "Untitled project." COOLLAB_FILE_EXTENSION;
}

auto backup_project() -> std::filesystem::path
{
    return Cool::Path::user_data() / "Backup project." COOLLAB_FILE_EXTENSION;
}

} // namespace Lab::Path
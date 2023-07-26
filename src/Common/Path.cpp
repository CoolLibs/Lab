#include "Path.h"
#include "Cool/Path/Path.h"
#include "ProjectManager/FileExtension.h"
#include "exe_path/exe_path.h"

namespace Lab::Path {

auto backup_project() -> std::filesystem::path
{
    return Cool::Path::user_data() / "backup." COOLLAB_FILE_EXTENSION;
}

} // namespace Lab::Path
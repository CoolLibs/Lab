#include "coollab_version.hpp"
#include "Cool/File/File.h"
#include "exe_path/exe_path.h"

namespace Lab {

auto coollab_version() -> std::string
{
    return Cool::File::file_name(exe_path::dir()).string();
}

} // namespace Lab
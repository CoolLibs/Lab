#include "gen_dump_string.h"
#include <string>
#include "Cool/Dump/gen_dump_string.h"
#include "coollab_version.h"

namespace Lab {

auto gen_dump_string() -> std::string
{
    return fmt::format(
        R"STR(---INFO---
CoolLab version   : {}
{}
)STR",
        coollab_version(),
        Cool::gen_dump_string()
    );
}

} // namespace Lab
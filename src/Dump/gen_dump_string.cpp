#include "gen_dump_string.h"
#include <string>
#include "Cool/Dump/gen_dump_string.h"
#include "coollab_version.hpp"

namespace Lab {

auto gen_dump_string() -> std::string
{
    return fmt::format(
        R"STR(---INFO---
Coollab version   : {}
{}
)STR",
        coollab_version(),
        Cool::gen_dump_string()
    );
}

} // namespace Lab
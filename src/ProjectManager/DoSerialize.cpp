///
/// This is in a separate .cpp file because
/// including "cereal/archives/json.hpp" slows down
/// compilation by A LOT. So we want to avoid recompiling
/// this file as much as possible.
///

#include "DoSerialize.h"
#include "Cool/Serialization/Serialization.h"
//
#include "cereal/archives/json.hpp"

namespace Lab {

auto serialize(Project const& project, std::filesystem::path const& path) -> bool
{
    return Cool::Serialization::save<Project, cereal::JSONOutputArchive>(project, path, "Project");
}

} // namespace Lab
///
/// We put all our serialization here in a separate .cpp file
/// because including "cereal/archives/json.hpp" slows down
/// compilation by A LOT. So we want to avoid recompiling
/// this file as much as possible.
///

#include "Cool/Serialization/Serialization.h"
#include "SProject.h"

//
#include "cereal/archives/json.hpp"

namespace Lab {

auto serialize(Project const& project, std::filesystem::path const& path) -> bool
{
    return Cool::Serialization::save<Project, cereal::JSONOutputArchive>(project, path, "Project");
}

auto deserialize(Project& project, std::filesystem::path const& path) -> Cool::OptionalErrorMessage
{
    return Cool::Serialization::load<Project, cereal::JSONInputArchive>(project, path);
}

} // namespace Lab
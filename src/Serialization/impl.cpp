///
/// We put all our serialization here in a separate .cpp file
/// because including "cereal/archives/json.hpp" slows down
/// compilation by A LOT. So we want to avoid recompiling
/// this file as much as possible.
///

#include <cereal/types/polymorphic.hpp>
#include "Cool/Serialization/Serialization.h"
#include "Module_Nodes/Module_Nodes.h"
#include "SNodesCategoryConfig.h"
#include "SProject.h"

//
#include "cereal/archives/json.hpp"

namespace Lab {

auto do_save(Project const& project, std::filesystem::path const& path) -> bool
{
    return Cool::Serialization::save<Project, cereal::JSONOutputArchive>(project, path, "Project");
}
auto do_load(Project& project, std::filesystem::path const& path) -> Cool::OptionalErrorMessage
{
    return Cool::Serialization::load<Project, cereal::JSONInputArchive>(project, path);
}

auto do_save(NodesCategoryConfig const& config, std::filesystem::path const& path) -> bool
{
    return Cool::Serialization::save<NodesCategoryConfig, cereal::JSONOutputArchive>(config, path);
}
auto do_load(NodesCategoryConfig& config, std::filesystem::path const& path) -> Cool::OptionalErrorMessage
{
    return Cool::Serialization::load<NodesCategoryConfig, cereal::JSONInputArchive>(config, path);
}

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::Module_Nodes); // NOLINT
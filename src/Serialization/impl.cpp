///
/// We put all our serialization here in a separate .cpp file
/// because including "cereal/archives/json.hpp" slows down
/// compilation by A LOT. So we want to avoid recompiling
/// this file as much as possible.
///
#include <cereal/types/polymorphic.hpp>
#include "Cool/Serialization/Serialization.h"
#include "SNodesCategoryConfig.h"
#include "SNodesClipboard.h"
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

auto string_from_nodes_clipboard(NodesClipboard const& clipboard) -> std::string
{
    auto ss = std::stringstream{};
    {
        auto archive = cereal::JSONOutputArchive{ss};
        archive(cereal::make_nvp("Coollab copied nodes, you can paste this in Coollab to paste the nodes.", clipboard));
    } // archive actual work happens during its destruction
    return ss.str();
}
auto string_to_nodes_clipboard(std::string const& string) -> NodesClipboard
{
    auto clipboard = NodesClipboard{};
    {
        auto ss      = std::stringstream{string};
        auto archive = cereal::JSONInputArchive{ss};
        archive(clipboard);
    } // archive actual work happens during its destruction
    return clipboard;
}

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::Module_Compositing); // NOLINT
CEREAL_REGISTER_TYPE(Lab::Module_Particles);   // NOLINT
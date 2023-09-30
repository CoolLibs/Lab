#pragma once
#include <filesystem>
#include "Cool/Log/OptionalErrorMessage.h"
#include "Module_Nodes/NodesCategoryConfig.h"

namespace Lab {

auto serialize_nodes_cat_cfg(NodesCategoryConfig const& config, std::filesystem::path const& path) -> bool;
auto deserialize(NodesCategoryConfig& config, std::filesystem::path const& path) -> Cool::OptionalErrorMessage;

} // namespace Lab
#pragma once
#include <filesystem>
#include "Cool/Log/OptionalErrorMessage.h"
#include "NodesGraph/NodesCategoryConfig.h"

namespace Lab {

auto do_save(NodesCategoryConfig const&, std::filesystem::path const&) -> bool;
auto do_load(NodesCategoryConfig&, std::filesystem::path const&) -> Cool::OptionalErrorMessage;

} // namespace Lab
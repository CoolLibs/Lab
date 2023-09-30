#pragma once
#include <filesystem>
#include "Cool/Log/OptionalErrorMessage.h"
#include "Project.h"

namespace Lab {

auto serialize(Project const& project, std::filesystem::path const& path) -> bool;
auto deserialize(Project& project, std::filesystem::path const& path) -> Cool::OptionalErrorMessage;

} // namespace Lab
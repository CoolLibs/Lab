#pragma once
#include <filesystem>
#include "Cool/Log/OptionalErrorMessage.h"
#include "Project.h"

namespace Lab {

auto do_save(Project const&, std::filesystem::path const&) -> bool;
auto do_load(Project&, std::filesystem::path const&) -> Cool::OptionalErrorMessage;

} // namespace Lab
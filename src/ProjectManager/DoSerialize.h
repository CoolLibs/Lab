#pragma once
#include <filesystem>
#include "Project.h"

namespace Lab {

auto serialize(Project const& project, std::filesystem::path const& path) -> bool;

}
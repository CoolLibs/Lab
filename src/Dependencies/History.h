#pragma once
#include <cmd/cmd.hpp>
#include <cmd/imgui_and_ser20.hpp>
#include "CommandCore/ReversibleCommand.h"
#include "TestCommand.hpp"

namespace Lab {

using History = cmd::HistoryWithUiAndSerialization<TestCommand>;

} // namespace Lab
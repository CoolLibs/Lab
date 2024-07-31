#pragma once
#include <cmd/cmd.hpp>
#include <cmd/imgui_and_ser20.hpp>
#include "CommandCore/ReversibleCommand.h"

namespace Lab {

using History = cmd::HistoryWithUiAndSerialization<Lab::ReversibleCommand>;

}
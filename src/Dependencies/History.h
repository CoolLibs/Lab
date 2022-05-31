#pragma once

#include <cmd/cmd.hpp>
#include <cmd/imgui_and_cereal.hpp>
#include "Commands/Commands.h"

namespace Lab {

using History = cmd::HistoryWithUiAndSerialization<Lab::ReversibleCommand>;

}
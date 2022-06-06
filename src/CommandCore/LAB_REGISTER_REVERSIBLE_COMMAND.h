#pragma once

#include "CommandCore/PolymorphicReversibleCommand.h"

#define LAB_REGISTER_REVERSIBLE_COMMAND(CommandType)                     \
    CEREAL_REGISTER_TYPE(Lab::PolymorphicReversibleCommand<CommandType>) \
    CEREAL_REGISTER_POLYMORPHIC_RELATION(Lab::IReversibleCommand, Lab::PolymorphicReversibleCommand<CommandType>)

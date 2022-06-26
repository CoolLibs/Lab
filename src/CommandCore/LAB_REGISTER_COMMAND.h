#pragma once

#include "CommandCore/PolymorphicCommand.h"

#define LAB_REGISTER_COMMAND(CommandType)                      \
    CEREAL_REGISTER_TYPE(Lab::PolymorphicCommand<CommandType>) \
    CEREAL_REGISTER_POLYMORPHIC_RELATION(Lab::ICommand, Lab::PolymorphicCommand<CommandType>)

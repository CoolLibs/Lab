#pragma once

#include "CommandCore/PolymorphicCommand.h"

#if COOL_SERIALIZATION
#define LAB_REGISTER_COMMAND(CommandType)                      \
    CEREAL_REGISTER_TYPE(Lab::PolymorphicCommand<CommandType>) \
    CEREAL_REGISTER_POLYMORPHIC_RELATION(Lab::ICommand, Lab::PolymorphicCommand<CommandType>)
#else
#define LAB_REGISTER_COMMAND(CommandType)
#endif
#pragma once

#include "CommandCore/PolymorphicReversibleCommand.h"

#if COOL_SERIALIZATION
#define LAB_REGISTER_REVERSIBLE_COMMAND(CommandType)                     \
    CEREAL_REGISTER_TYPE(Lab::PolymorphicReversibleCommand<CommandType>) \
    CEREAL_REGISTER_POLYMORPHIC_RELATION(Lab::IReversibleCommand, Lab::PolymorphicReversibleCommand<CommandType>)
#else
#define LAB_REGISTER_REVERSIBLE_COMMAND(CommandType)
#endif
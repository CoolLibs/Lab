#pragma once
#include <ser20/archives/json.hpp>
#include <ser20/types/polymorphic.hpp>
#include "CommandCore/PolymorphicReversibleCommand.h"

#define LAB_REGISTER_REVERSIBLE_COMMAND(CommandType)                    \
    SER20_REGISTER_TYPE(Lab::PolymorphicReversibleCommand<CommandType>) \
    SER20_REGISTER_POLYMORPHIC_RELATION(Lab::IReversibleCommand, Lab::PolymorphicReversibleCommand<CommandType>)

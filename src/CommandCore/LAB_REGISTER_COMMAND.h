#pragma once
#include <ser20/archives/json.hpp>
#include <ser20/types/polymorphic.hpp>
#include "CommandCore/PolymorphicCommand.h"

#define LAB_REGISTER_COMMAND(CommandType)                     \
    SER20_REGISTER_TYPE(Lab::PolymorphicCommand<CommandType>) \
    SER20_REGISTER_POLYMORPHIC_RELATION(Lab::ICommand, Lab::PolymorphicCommand<CommandType>)

#pragma once

#include "Dependencies/TestCommand.hpp"
#include "ReversibleCommand.h"

namespace Lab {

class ReversibleCommandMerger_Ref {
public:
    static auto merge(
        const TestCommand& prev,
        const TestCommand& curr
    )
        -> std::optional<TestCommand>
    {
        return {};
    }
};

} // namespace Lab

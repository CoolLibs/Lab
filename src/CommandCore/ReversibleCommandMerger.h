#pragma once

#include "ReversibleCommand.h"

namespace Lab {

class ReversibleCommandMerger {
public:
    static auto merge(const ReversibleCommand& prev,
                      const ReversibleCommand& curr)
        -> std::optional<ReversibleCommand>
    {
        return curr->merge(*prev);
    }
};

} // namespace Lab

#include "can_convert.h"

namespace Lab {

auto can_convert(PrimitiveType from, PrimitiveType to) -> bool // NOLINT(readability-function-cognitive-complexity)
{
    if (from == to)
        return true;

#include "generated/can_convert.inl"
    return false; // NOLINT(*misleading-indentation)
}

} // namespace Lab
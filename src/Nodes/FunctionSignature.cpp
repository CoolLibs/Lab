#include "FunctionSignature.h"

namespace Lab {

auto missing_functions(FunctionSignature current, FunctionSignature desired) -> std::vector<FunctionSignature>
{
    if (current == desired)
        return {};

    if (current.to == desired.to)
        return {
            FunctionSignature{
                .from = desired.from,
                .to   = current.from,
            }};

    if (current.from == desired.from)
        return {
            FunctionSignature{
                .from = current.to,
                .to   = desired.to,
            }};
}

} // namespace Lab
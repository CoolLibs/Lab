#pragma once
#include "CodeGenContext.h"
#include "FunctionSignature.h"
#include "PrimitiveType.h"

namespace Lab {

struct ImplicitConversions {
    std::optional<std::string> input;
    std::optional<std::string> output;

    [[nodiscard]] auto both_exist() const -> bool { return input && output; }
};

/// If a conversion function between `from` and `to` exists, generates it in the context and returns its name.
auto gen_implicit_conversion(PrimitiveType from, PrimitiveType to, CodeGenContext&)
    -> std::optional<std::string>;

/// Returns the conversions (if any) for the input and output of the signature in order to go from `current` to `desired`.
auto gen_implicit_conversions(FunctionSignature current, FunctionSignature desired, CodeGenContext&)
    -> ImplicitConversions;

} // namespace Lab

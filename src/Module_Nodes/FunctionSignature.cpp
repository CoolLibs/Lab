#include "FunctionSignature.h"

namespace Lab {

auto make_complete_function_signature(MainFunctionSignature const& signature)
    -> CompleteFunctionSignature
{
    auto res = CompleteFunctionSignature{};

    res.output_type = signature.signature.to;

    std::transform(
        signature.parameter_names.begin(), signature.parameter_names.end(),
        std::back_inserter(res.parameters),
        [&](std::string const& name) {
            return ParamDesc{
                .name = name,
                .type = signature.signature.from,
            };
        }
    );

    return res;
}

} // namespace Lab
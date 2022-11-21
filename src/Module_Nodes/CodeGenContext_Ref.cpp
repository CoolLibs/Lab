#include "CodeGenContext_Ref.h"
#include "CodeGen.h"
#include "CodeGen_default_function.h"

namespace Lab {

auto InputFunctionGenerator_Ref::operator()(FunctionSignature desired_signature)
    -> tl::expected<Function, std::string>
{
    return gen_desired_function(
        desired_signature,
        _context.graph.input_node_id(_node.get().main_input_pin().id()),
        _context
    );
}

auto DefaultFunctionGenerator_Ref::operator()(FunctionSignature desired_signature)
    -> tl::expected<Function, std::string>
{
    return gen_default_function(
        desired_signature,
        _context.already_generated_functions
    );
}

} // namespace Lab
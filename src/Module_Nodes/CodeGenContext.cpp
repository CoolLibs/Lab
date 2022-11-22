#include "CodeGenContext.h"
#include "CodeGen.h"
#include "CodeGen_default_function.h"

namespace Lab {

namespace internal {

void AlreadyGeneratedFunctions::push(std::string const& name)
{
    _names.push_back(name);
}

auto AlreadyGeneratedFunctions::has_already_been_generated(std::string const& name) const -> bool
{
    return std::find(_names.begin(), _names.end(), name) != _names.end();
}

} // namespace internal

auto CodeGenContext::push_function(Function const& function) -> std::string
{
    if (!_already_generated_functions.has_already_been_generated(function.name))
    {
        _already_generated_functions.push(function.name);
        _code += function.implementation;
    }
    else
    {
        _code += fmt::format("/* Function \"{}\" has already been generated. */", function.name);
    }

    return function.name;
}

auto InputFunctionGenerator_Ref::operator()(FunctionSignature desired_signature)
    -> ExpectedFunctionName
{
    return gen_desired_function(
        desired_signature,
        _context.get().graph().input_node_id(_node.get().main_input_pin().id()),
        _context
    );
}

auto DefaultFunctionGenerator_Ref::operator()(FunctionSignature desired_signature)
    -> ExpectedFunctionName
{
    return gen_default_function(
        desired_signature,
        _context
    );
}

} // namespace Lab
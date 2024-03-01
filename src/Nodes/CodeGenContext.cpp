#include "CodeGenContext.h"

namespace Lab {

void CodeGenContext::AlreadyGeneratedSymbols::push(std::string const& name)
{
    _names.push_back(name);
}

auto CodeGenContext::AlreadyGeneratedSymbols::has_already_been_generated(std::string const& name) const -> bool
{
    return std::find(_names.begin(), _names.end(), name) != _names.end();
}

auto CodeGenContext::push_function(FunctionDefinition const& function) -> std::string
{
    if (!_already_generated_functions.has_already_been_generated(function.name))
    {
        _already_generated_functions.push(function.name);
        _code += function.definition;
    }
    else
    {
        _code += fmt::format("/* Function \"{}\" has already been generated. */", function.name);
    }

    return function.name;
}

void CodeGenContext::push_struct(Struct const& structeuh)
{
    if (!_already_generated_structs.has_already_been_generated(structeuh.name))
    {
        _already_generated_structs.push(structeuh.name);
        _code += fmt::format("struct {}{};", structeuh.name, structeuh.body);
    }
    else
    {
        _code += fmt::format("/* Struct \"{}\" has already been generated. */", structeuh.name);
    }
}

} // namespace Lab
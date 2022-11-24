#include "CodeGenContext.h"

namespace Lab {

void CodeGenContext::AlreadyGeneratedFunctions::push(std::string const& name)
{
    _names.push_back(name);
}

auto CodeGenContext::AlreadyGeneratedFunctions::has_already_been_generated(std::string const& name) const -> bool
{
    return std::find(_names.begin(), _names.end(), name) != _names.end();
}

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

} // namespace Lab
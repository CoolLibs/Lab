#include "Function.h"

namespace Lab {

void AlreadyGeneratedFunctions::push(std::string const& name)
{
    _names.push_back(name);
}

auto AlreadyGeneratedFunctions::has_already_been_generated(std::string const& name) const -> bool
{
    return std::find(_names.begin(), _names.end(), name) != _names.end();
}

Function::Function(Function_Data const& data, AlreadyGeneratedFunctions& check)
    : name{data.name}
{
    // Only generate the implementation code if it has not yet been generated
    if (!check.has_already_been_generated(data.name))
    {
        check.push(data.name);
        implementation = data.implementation;
    }
    else
    {
        implementation = fmt::format("/* Function \"{}\" has already been generated. */", data.name);
    }
};

} // namespace Lab
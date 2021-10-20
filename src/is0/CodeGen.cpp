#include "CodeGen.h"
#include <numeric>

namespace CodeGen {

std::string function_name(const FnNameParams& p)
{
    return std::string{p.node_template_name} + "_" + std::to_string(p.node_uuid);
}

std::string function_signature(const FnSignatureParams& p)
{
    return "float " + function_name(p.fn_name_params) + std::string{p.sdf_param_declaration};
}

std::string function_declaration(const FnSignatureParams& p)
{
    return function_signature(p) + ";";
}

std::string function_definition(const FnDefinitionParams& p)
{
    return function_signature(p.fn_signature_params) + std::string{p.body};
}

std::string parameters_definitions(const Cool::ParameterList& list)
{
    return std::accumulate(list->begin(), list->end(), std::string{""}, [](const std::string& acc, const auto& param) {
        return acc + parameter_definition_any(param) + '\n';
    });
}

// std::string parameter_definition(const Cool::Parameter::Any& param)
// {
//     const auto& name = std::visit([](const auto&& param) { return param.name(); }, param);
//     return name;
// }

} // namespace CodeGen
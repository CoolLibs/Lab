#include "CodeGen.h"
#include <Cool/String/String.h>
#include <numeric>
#include "NodeEditorU.h"

namespace CodeGen {

std::string full_shader_code(const std::vector<Node>& nodes, const std::vector<Link>& links, const std::vector<NodeTemplate>& node_templates)
{
    using namespace std::string_literals;
    std::stringstream declarations;
    std::stringstream definitions;
    for (const auto& node : nodes) {
        const auto& node_template       = *std::ranges::find_if(node_templates, [&](const NodeTemplate& node_template) {
            return node_template.name == node.node_template_name;
        });
        const auto  fn_signature_params = FnSignatureParams{.fn_name_params = FnNameParams{
                                                               .node_template_name = node.node_template_name,
                                                               .node_uuid          = node.uuid},
                                                           .sdf_param_declaration = node_template.vec3_input_declaration};
        declarations << function_declaration(fn_signature_params);
        declarations << '\n';
        std::vector<std::pair<std::string, std::string>> sdf_identifiers;
        sdf_identifiers.reserve(node.input_pins.size());
        for (size_t i = 0; i < node.input_pins.size(); ++i) {
            const Node* input_node = NodeEditorU::find_input_node(node.input_pins[i], nodes, links);
            sdf_identifiers.push_back(std::make_pair(
                node_template.sdf_identifiers[i],
                input_node ? function_name({input_node->node_template_name, input_node->uuid}) : "cool_default_sdf"s));
        }
        definitions << function_definition(FnDefinitionParams{
            .fn_signature_params = fn_signature_params,
            .body                = function_body(
                node.parameter_list,
                node_template.code_template,
                sdf_identifiers)});
        definitions << "\n\n";
    }
    return declarations.str() + '\n' + definitions.str();
}

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

std::string function_body(const Cool::ParameterList& list, const std::string& code_template, const std::vector<std::pair<std::string, std::string>>& sdf_identifiers)
{
    return "{\n" +
           parameters_definitions(list) +
           Cool::String::replace({code_template, sdf_identifiers, "${", "}"}) +
           "\n}";
}

std::string parameters_definitions(const Cool::ParameterList& list)
{
    return std::accumulate(list->begin(), list->end(), std::string{""}, [](const std::string& acc, const auto& param) {
        return acc + parameter_definition_any(param) + '\n';
    });
}

std::string parameter_definition_any(const Cool::Parameter::Any& param)
{
    return std::visit([](auto&& param) { return parameter_definition(param); }, param);
}

} // namespace CodeGen
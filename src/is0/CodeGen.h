#pragma once

#include <Cool/Parameter/ParameterList.h>
#include <Cool/Uuid/Uuid.h>

namespace CodeGen {

struct FnNameParams {
    std::string_view node_template_name;
    Cool::Uuid       node_uuid;
};

struct FnSignatureParams {
    const FnNameParams& fn_name_params;
    std::string_view    sdf_param_declaration;
};

struct FnDefinitionParams {
    const FnSignatureParams& fn_signature_params;
    std::string_view         body;
};

std::string function_name(const FnNameParams& p);
std::string function_signature(const FnSignatureParams& p);
std::string function_declaration(const FnSignatureParams& p);
std::string function_definition(const FnDefinitionParams& p);

std::string parameters_definitions(const Cool::ParameterList& list);
std::string parameter_definition_any(const Cool::Parameter::Any& param);
template<Cool::ParameterDesc Desc>
std::string parameter_definition(const Cool::Parameter_Base<Desc>& param);
template<typename T>
std::string type_to_string();
template<typename T>
std::string value_to_string(const T&& x);

} // namespace CodeGen

#include "CodeGen.tpp"

TEST_CASE("[is0::CodeGen] Function generation")
{
    // Given
    const auto name              = std::string{"my_sdf"};
    const auto name_params       = CodeGen::FnNameParams{name,
                                                   Cool::Uuid{42}};
    const auto signature_params  = CodeGen::FnSignatureParams{name_params,
                                                             "(vec3 pos)"};
    const auto definition_params = CodeGen::FnDefinitionParams{signature_params,
                                                               "{ return 1.; }"};
    // Then
    CHECK(CodeGen::function_name(name_params) == "my_sdf_42");
    CHECK(CodeGen::function_signature(signature_params) == "float my_sdf_42(vec3 pos)");
    CHECK(CodeGen::function_declaration(signature_params) == "float my_sdf_42(vec3 pos);");
    CHECK(CodeGen::function_definition(definition_params) == "float my_sdf_42(vec3 pos){ return 1.; }");
}

TEST_CASE("[is0::CodeGen] Parameter definition")
{
    // Given
    const auto param_int  = Cool::Parameter::Int{{.name = "my_param", .default_value = 11}};
    const auto param_vec2 = Cool::Parameter::Vec2{{.name = "my_param", .default_value = {1.f, 2.f}}};
    auto       param_list = Cool::ParameterList{};
    param_list->push_back(param_int);
    param_list->push_back(param_vec2);
    // Then
    CHECK(CodeGen::parameter_definition(param_int) == "const int my_param = 11;");
    CHECK(CodeGen::parameter_definition(param_vec2) == "const vec2 my_param = vec2(1.000000, 2.000000);");
    CHECK(CodeGen::parameters_definitions(param_list) == R"(const int my_param = 11;
const vec2 my_param = vec2(1.000000, 2.000000);
)");
}
namespace CodeGen {

template<Cool::ParameterDesc Desc>
std::string parameter_definition(const Cool::Parameter_Base<Desc>& param)
{
    return "const " +
           type_to_string<typename Desc::Value>() + " " +
           convert_to_valid_glsl_name(param.name()) +
           " = " + value_to_string(*param) + ";";
}

template<typename T>
std::string type_to_string()
{
    if constexpr (std::is_same_v<T, bool>) {
        return "bool";
    }
    if constexpr (std::is_same_v<T, int>) {
        return "int";
    }
    if constexpr (std::is_same_v<T, float>) {
        return "float";
    }
    if constexpr (std::is_same_v<T, glm::vec2>) {
        return "vec2";
    }
    if constexpr (std::is_same_v<T, glm::vec3>) {
        return "vec3";
    }
    if constexpr (std::is_same_v<T, glm::vec4>) {
        return "vec4";
    }
}

template<typename T>
std::string value_to_string(const T&& x)
{
    if constexpr (std::is_same_v<T, bool>) {
        return x ? "true" : "false";
    }
    if constexpr (std::is_same_v<T, int>) {
        return std::to_string(x);
    }
    if constexpr (std::is_same_v<T, float>) {
        return std::to_string(x);
    }
    if constexpr (std::is_same_v<T, glm::vec2>) {
        return glm::to_string(x);
    }
    if constexpr (std::is_same_v<T, glm::vec3>) {
        return glm::to_string(x);
    }
    if constexpr (std::is_same_v<T, glm::vec4>) {
        return glm::to_string(x);
    }
}

} // namespace CodeGen
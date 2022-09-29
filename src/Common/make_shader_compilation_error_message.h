#pragma once

#include <Cool/Log/Message.h>

namespace Lab {

inline auto make_shader_compilation_error_message(
    const std::string& module_name,
    const std::string& shader_name,
    const std::string& message
) -> Cool::Message
{
    return Cool::Message{
        .category = module_name,
        .message  = fmt::format("\"{}\"\n{}", shader_name, message),
        .severity = Cool::MessageSeverity::Error,
    };
}

} // namespace Lab
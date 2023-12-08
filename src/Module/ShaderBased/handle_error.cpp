#include "handle_error.h"
#include <Common/make_shader_compilation_error_message.h>
#include <Cool/Log/MessageSender.h>
#include <Cool/Log/OptionalErrorMessage.h>
#include <Module/Module.h>

namespace Lab {

void handle_error(Cool::OptionalErrorMessage const& maybe_err, std::string name, Cool::MessageSender& _shader_compilation_error)
{
    maybe_err.send_error_if_any(_shader_compilation_error, [&](const std::string& msg) {
        return make_shader_compilation_error_message(name, "", msg);
    });
}

} // namespace Lab
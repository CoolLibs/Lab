#include "handle_error.h"
#include <Common/make_shader_compilation_error_message.h>
#include <Cool/Log/MessageSender.h>
#include <Cool/Log/OptionalErrorMessage.h>
#include <Module/Module.h>

namespace Lab {

void handle_error(Cool::OptionalErrorMessage const& maybe_err, std::string const& module_name, Cool::MessageSender& message_sender)
{
    maybe_err.send_error_if_any(message_sender, [&](std::string const& msg) {
        return make_shader_compilation_error_message(module_name, "", msg);
    });
}

} // namespace Lab
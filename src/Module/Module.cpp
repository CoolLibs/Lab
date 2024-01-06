#include "Module.h"

namespace Lab {

void Module::log_module_error(Cool::OptionalErrorMessage const& maybe_err, Cool::MessageSender& message_sender) const
{
    maybe_err.send_error_if_any(message_sender, [&](std::string const& msg) {
        return Cool::Message{
            .category = name(),
            .message  = msg,
            .severity = Cool::MessageSeverity::Error,
        };
    });
}

} // namespace Lab
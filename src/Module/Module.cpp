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

auto Module::needs_to_rerender() const -> bool
{
    return _needs_to_rerender_flag.is_dirty() || std::any_of(_modules_that_we_depend_on.begin(), _modules_that_we_depend_on.end(), [&](auto&& module) {
               return module->needs_to_rerender();
           });
};

} // namespace Lab
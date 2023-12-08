#pragma once

#include <Cool/Log/MessageSender.h>
#include <Cool/Log/OptionalErrorMessage.h>

namespace Lab {

void handle_error(Cool::OptionalErrorMessage const& maybe_err, std::string name, Cool::MessageSender& _shader_compilation_error);

} // namespace Lab
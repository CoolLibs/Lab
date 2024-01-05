#pragma once
#include <Cool/Log/MessageSender.h>
#include <Cool/Log/OptionalErrorMessage.h>

namespace Lab {

void handle_error(Cool::OptionalErrorMessage const&, std::string const& module_name, Cool::MessageSender&);

} // namespace Lab
#pragma once

namespace Lab {

struct Function {
    std::string name;
    std::string implementation;
};

/// Function name, or an error message in the unexpected case.
using ExpectedFunctionName = tl::expected<std::string, std::string>;

} // namespace Lab

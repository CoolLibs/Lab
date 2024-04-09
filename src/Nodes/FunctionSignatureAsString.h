#pragma once

namespace Lab {

struct FunctionSignatureAsString {
    std::string return_type;
    std::string name;
    std::string arguments_list; // e.g. "float bob, vec2 yo" NB: It does NOT include the parentheses.
};

} // namespace Lab
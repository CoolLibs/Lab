#pragma once

namespace Lab {

/// Returns a string with only letters and numbers, no underscores.
/// This string does not start with a number.
auto valid_glsl(std::string s) -> std::string;

} // namespace Lab
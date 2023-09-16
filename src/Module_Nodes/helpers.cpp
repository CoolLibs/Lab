#include "helpers.h"

namespace Lab {

static auto is_not_alphanumeric(char c) -> bool
{
    return !( // NOLINT(readability-simplify-boolean-expr)
        ('0' <= c && c <= '9')
        || ('a' <= c && c <= 'z')
        || ('A' <= c && c <= 'Z')
    );
}

auto valid_glsl(std::string s)
    -> std::string
{
    // For glsl variable name rules, see https://www.informit.com/articles/article.aspx?p=2731929&seqNum=3, section "Declaring Variables".
    s.erase(std::remove_if(s.begin(), s.end(), &is_not_alphanumeric), s.end()); // `s` can only contain letters and numbers (and _, but two consecutive underscores is invalid so we don't allow any: this is the simplest way to enforce that rule, at the cost of slightly uglier names)
    return "_" + s;                                                             // We need a prefix to make sure `s` does not start with a number.
}

} // namespace Lab
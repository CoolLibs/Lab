#if !DOCTEST_CONFIG_DISABLE
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include "Cool/Core/InitConfig.h"

// NB: the tests are defined alongside the file that they test, not here.
auto main() -> int
{
    auto const init_config = Cool::InitConfig{};
    Cool::Path::initialize_root(init_config.root_path);
    Cool::Path::initialize_cool_res(init_config.cool_res_path);
    return doctest::Context{}.run();
}

#else
auto main() -> int {}
#endif
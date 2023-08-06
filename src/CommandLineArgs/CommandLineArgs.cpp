#include "CommandLineArgs.h"

namespace Lab {

void CommandLineArgs::init(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i) // Skip the first argument, since we don't care about the path of the Coollab executable.
    {
        _args.emplace_back(argv[i]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }
}

} // namespace Lab
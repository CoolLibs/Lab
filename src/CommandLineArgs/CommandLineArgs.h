#pragma once

namespace Lab {

class CommandLineArgs {
public:
    [[nodiscard]] auto get() const -> auto const& { return _args; }
    void               init(int argc, char** argv);

private:
    std::vector<std::string> _args{};
};

inline auto command_line_args() -> CommandLineArgs&
{
    static auto instance = CommandLineArgs{};
    return instance;
}

} // namespace Lab
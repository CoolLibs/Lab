#pragma once

namespace Lab {

struct Function_Data {
    std::string name;
    std::string definition;
};

class AlreadyGeneratedFunctions {
public:
    AlreadyGeneratedFunctions(){};

private:
    friend struct Function;
    void push(std::string const& name);
    auto has_already_been_generated(std::string const& name) const -> bool;

private:
    std::vector<std::string> _names;
};

struct Function {
    Function(Function_Data const&, AlreadyGeneratedFunctions&);

    std::string name;
    std::string definition{};
};

} // namespace Lab

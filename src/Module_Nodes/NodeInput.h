#pragma once

namespace Lab {

enum class InputType {
    Int,
    Float,
    Bool, // TODO generate one corrsponding to each variable type
    // ...
    Image,
    Colorizer,
    Noise2D,
    Noise3D,
};

class NodeInput {
public:
    NodeInput(std::string_view name, InputType type)
        : _name{name}
        , _type{type}
    {}

    auto name() const -> const std::string& { return _name; }
    auto type() const -> InputType { return _type; }

private:
    std::string _name;
    InputType   _type;
};

} // namespace Lab

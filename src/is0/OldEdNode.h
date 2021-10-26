#pragma once

#include <entt/entt.hpp>

using CodeGenerator = std::function<std::string(entt::entity)>;
using UiWidgets     = std::function<bool(entt::entity)>;

struct OldEdNode {
    std::string name;
    std::string fn_name;
    ed::NodeId  node_id;
    std::string fn_declaration;
    std::string fn_implementation;

    CodeGenerator gen_source_code;
    UiWidgets     show_widgets;
};

struct InputPinSingle {
    ed::PinId id;

    InputPinSingle(ed::PinId id)
        : id(id) {}
};

struct OutputPinSingle {
    ed::PinId id;

    OutputPinSingle(ed::PinId id)
        : id(id) {}
};

struct ShapeNode {
    OutputPinSingle output_pin;
};

struct ModifierNode {
    // entt::entity input_node;
    InputPinSingle  input_pin;
    OutputPinSingle output_pin;
};

struct IsTerminalNode {
};
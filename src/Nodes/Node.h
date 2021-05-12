#pragma once

#include <entt/entt.hpp>

struct NodeInfo {
	std::string name;
	std::string fn_name;
	ed::NodeId node_id;
};

struct NodeCode {
	std::string fn_declaration;
	std::string fn_implementation;
};

struct InputPinSingle {
	ed::PinId id;

	InputPinSingle(ed::PinId id) : id(id) {}
};

struct OutputPinSingle {
	ed::PinId id;

	OutputPinSingle(ed::PinId id) : id(id) {}
};

struct ShapeNode {
	OutputPinSingle output_pin;
};

struct ModifierNode {
	// entt::entity input_node; 
	InputPinSingle input_pin;
	OutputPinSingle output_pin;
};

using CodeGenerator = std::function<std::string(entt::entity)>;

struct NodeCodeGenerator {
	CodeGenerator gen_source_code;
};

struct IsTerminalNode {};
#pragma once

#include <entt/entt.hpp>

struct NodeInfo {
	std::string name;
	std::string fn_name;
};

struct NodeCode {
	std::string fn_declaration;
	std::string fn_implementation;
};

struct ShapeNode {
	entt::entity output_pin;
};

struct ModifierNode {
	entt::entity input_node;
	entt::entity input_pin;
	entt::entity output_pin;
};

using CodeGenerator = std::function<std::string(entt::entity)>;

struct NodeCodeGenerator {
	CodeGenerator gen_source_code;
};

struct IsTerminalNode {};
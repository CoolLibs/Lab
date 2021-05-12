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

struct ModifierNode {
	entt::entity input_node;
};

using CodeGenerator = std::function<std::string(entt::entity)>;

struct NodeCodeGenerator {
	CodeGenerator gen_source_code;
};

struct IsTerminalNode {};
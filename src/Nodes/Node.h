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

using CodeGenerator = std::function<std::string(entt::entity)>;

struct NodeCodeGenerator {
	CodeGenerator gen_source_code;
};

struct IsTerminalNode {};

struct ModifierNode {

};

class Input {
public:
	virtual std::string to_glsl_string() const = 0;
};

class Input_Vec3 : public Input{
public:
	std::string to_glsl_string() const override {
		return "(vec3 pos)";
	}
};

class Output {
public:
	virtual std::string to_glsl_string() const = 0;
	virtual bool is_terminal(const entt::registry& R) const = 0;
};

class Output_Float : public Output {
public:
	std::string to_glsl_string() const override {
		return "float";
	}

	bool is_terminal(const entt::registry& R) const override {
		return !R.valid(out_node);
	}

private:
	entt::entity out_node = entt::null;
};

struct Node {
	std::string name;
	std::string glsl_body;
	std::unique_ptr<Input> input;
	std::unique_ptr<Output> output;
};

bool is_terminal(entt::registry& R, entt::entity e) {
	const auto& node = R.get<Node>(e);
	return node.output->is_terminal(R);
}

std::uint32_t id(entt::entity e) {
	return static_cast<std::uint32_t>(e);
}

std::string function_name(entt::registry& R, entt::entity e) {
	const auto& node = R.get<Node>(e);
	return node.name + "__" + std::to_string(id(e));
}

std::string function_signature(entt::registry& R, entt::entity e) {
	const auto& node = R.get<Node>(e);
	return node.output->to_glsl_string() + " " + function_name(R, e) + node.input->to_glsl_string();
}

std::string function_declaration(entt::registry& R, entt::entity e) {
	return function_signature(R, e) + ";\n";
}

std::string function_implementation(entt::registry& R, entt::entity e) {
	const auto& node = R.get<Node>(e);
	return function_signature(R, e) + " {\n"
		 + node.glsl_body
		 + "\n};\n";
}

entt::entity create_shape_node(entt::registry& R, std::string name, std::string glsl_body) {
	entt::entity e = R.create();
	Node node;
	node.name = name;
	node.glsl_body = glsl_body;
	node.input = std::make_unique<Input_Vec3>();
	node.output = std::make_unique<Output_Float>();
	R.emplace<Node>(e, std::move(node));
	return e;
}

using namespace Cool;

#include "Node.h"

size_t Node::next_node_id = 1;

Node::Node(Type return_type, std::string_view name)
	: _return_type(return_type), _name(name), _id(next_node_id++)
{}

std::string Node::function_name() {
	return _name + "_" + std::to_string(_id);
}

std::string Node::function_signature() {
	return to_string(_return_type) + " " + function_name() + "(vec3 pos)";
}

std::string Node::function_declaration() {
	return function_signature() + ";\n";
}

std::string Node::function_implementation() {
	return function_signature() + " {\n"
		 + function_body()
		 + "\n};\n";
}

std::string Node::to_string(Type type) {
	switch (type)
	{
	case Type::Float:
		return "float";
	case Type::Vec3:
		return "vec3";
	default:
		Log::Error("[Node::to_string] Unknown type");
		return "ERROR";
	}
}
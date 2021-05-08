using namespace Cool;

#include "Node.h"

size_t Node::next_node_id = 1;

Node::Node(std::string_view name)
	: _name(name), _id(next_node_id++)
{}

std::string Node::function_name() {
	return _name + "_" + std::to_string(_id);
}

std::string Node::function_signature() {
	return output_type() + " " + function_name() + inputs();
}

std::string Node::function_declaration() {
	return function_signature() + ";\n";
}

std::string Node::function_implementation() {
	return function_signature() + " {\n"
		 + function_body()
		 + "\n};\n";
}
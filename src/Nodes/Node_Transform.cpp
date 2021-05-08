#include "Node_Transform.h"

Node_Transform::Node_Transform()
	: ModifierNode("Transform")
{}

std::string Node_Transform::function_body() {
	return "return pos - vec3(" + std::to_string(id()) +");";
}
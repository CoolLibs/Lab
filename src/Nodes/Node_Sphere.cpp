#include "Node_Sphere.h"

Node_Sphere::Node_Sphere()
	: ShapeNode("Sphere")
{}

bool Node_Sphere::is_terminal() {
	return true; // TODO
}

std::string Node_Sphere::function_body() {
	return "return length(pos + vec3(" + std::to_string(id()) +")) - 1;";
}
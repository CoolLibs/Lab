#pragma once

#include "Node.h"

class Node_Sphere : public ShapeNode {
public:
	Node_Sphere();
	std::string function_body() override;
};
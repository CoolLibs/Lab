#pragma once

#include "Node.h"

class Node_Sphere : public Node {
public:
	Node_Sphere();
	std::string function_body() override;
};
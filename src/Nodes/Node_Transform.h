#pragma once

#include "Node.h"

class Node_Transform : public ModifierNode {
public:
	Node_Transform();
	std::string function_body() override;
};
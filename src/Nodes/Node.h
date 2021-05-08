#pragma once

class Node {
public:
	Node(std::string_view name);

	inline std::string_view name() const { return _name; }
	inline size_t id() const { return _id; }

	virtual std::string function_body() = 0;
	virtual std::string inputs() = 0;
	virtual std::string output_type() = 0;
	std::string function_name();
	std::string function_signature();
	std::string function_declaration();
	std::string function_implementation();

private:
	std::string _name;
	size_t _id;
	static size_t next_node_id;
};

class ModifierNode : public Node {
public:
	ModifierNode(std::string_view name)
		: Node(name)
	{}

	std::string inputs() override {
		return "(vec3 pos)";
	}

	std::string output_type() override {
		return "vec3";
	}
};

class ShapeNode : public Node {
public:
	ShapeNode(std::string_view name)
		: Node(name)
	{}

	std::string inputs() override {
		return "(vec3 pos)";
	}

	std::string output_type() override {
		return "float";
	}
};

class CombinerNode : public Node {
public:
	CombinerNode(std::string_view name)
		: Node(name)
	{
		_input_nodes_ids.push_back(0); // TODO remove me
		_input_nodes_ids.push_back(1);
		_input_nodes_ids.push_back(2);
	}

	std::string inputs() override {
		std::string s = "(float d0";
		for (size_t i = 1; i < _input_nodes_ids.size(); ++i) {
			s += ", float d" + std::to_string(i);
		}
		s += ")";
		return s;
	}

	std::string output_type() override {
		return "float";
	}

private:
	std::vector<size_t> _input_nodes_ids;
};
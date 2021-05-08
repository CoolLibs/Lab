#pragma once

enum class Type {
	Float,
	Vec3
};

class Node {
public:
	Node(Type return_type, std::string_view name);

	inline std::string_view name() const { return _name; }
	inline size_t id() const { return _id; }

	virtual std::string function_body() = 0;
	std::string function_name();
	std::string function_signature();
	std::string function_declaration();
	std::string function_implementation();

	static std::string to_string(Type type);

private:
	Type _return_type;
	std::string _name;
	size_t _id;
	static size_t next_node_id;
};
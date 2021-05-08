#pragma once

#include "Node.h"
#include <entt/entt.hpp>

namespace NodeFactory {

std::string fn_signature(std::string output, std::string name, std::string inputs) {
	return output + " " + name + inputs;
}

std::string fn_declaration(std::string signature) {
	return signature + ";\n";
}

std::string fn_implementation(std::string signature, std::string body) {
	return signature + " {\n" + body + "\n}\n";
}

inline entt::entity node(entt::registry& R, std::string name) {
	entt::entity e = R.create();
	R.emplace<NodeInfo>(e,
		/*name*/ name,
		/*fn_name*/ name + "__" + std::to_string(static_cast<std::uint32_t>(e))
	);
	return e;
}

inline entt::entity shape(entt::registry& R, std::string name, std::string fn_body) {
	entt::entity e = node(R, name);
	std::string fn_name = R.get<NodeInfo>(e).fn_name;
	std::string signature = fn_signature("float", fn_name, "(vec3 pos)");
	R.emplace<NodeCode>(e,
		/*fn_declaration*/ fn_declaration(signature),
		/*fn_implementation*/ fn_implementation(signature, fn_body)
	);
	R.emplace<IsTerminalNode>(e);
	return e;
}

inline entt::entity sphere(entt::registry& R) {
	static int coutn = 0;
	return shape(R,
		"Sphere",
		"return length(pos -vec3(" + std::to_string(coutn++) + ")) - 1;"
	);
}

inline entt::entity cube(entt::registry& R) {
	return shape(R,
		"Cube",
		R"V0G0N(
	vec3 q = abs(pos) - 1.;
	return length(max(q, 0.)) + min(max(q.x, max(q.y, q.z)), 0.);
	)V0G0N"
	);
}

} // namespace CreateNode
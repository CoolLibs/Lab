#pragma once

#include <entt/entt.hpp>

struct PinParentNode {
	entt::entity parent_node;
};

namespace PinFactory {

inline entt::entity pin(entt::registry& R, entt::entity parent_node) {
	entt::entity e = R.create();
	R.emplace<PinParentNode>(e, parent_node);
	return e;
}

} // namespace PinFactory
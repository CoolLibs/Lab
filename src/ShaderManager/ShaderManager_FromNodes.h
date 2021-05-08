#pragma once

#include "ShaderManager.h"

class ShaderManager_FromNodes : public ShaderManager {
public:
	inline void update() override {}
	inline bool is_valid() const override { return false; } // TODO
	void ImGui() override;

private:

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive) {

	}
};
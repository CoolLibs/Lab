#pragma once

#include "ShaderManager_FromNodes.h"
#include "ShaderManager_FromText.h"

class ShaderManagerManager {
public:
	void ImGui_windows();

	ShaderManager& operator*() { if (_use_nodes) return _from_nodes; else return _from_text; }
	ShaderManager* operator->() { return &operator*(); }

private:
	bool _use_nodes = false;
	ShaderManager_FromNodes _from_nodes;
	ShaderManager_FromText _from_text;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			cereal::make_nvp("Use Nodes ?", _use_nodes),
			cereal::make_nvp("From Nodes", _from_nodes),
			cereal::make_nvp("From Text", _from_text)
		);
	}
};
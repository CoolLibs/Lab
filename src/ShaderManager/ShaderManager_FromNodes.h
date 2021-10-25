#pragma once

#include "ShaderManager.h"
#include "is0/EdNodeEditor.h"
#include "is0/NodeManager.h"

class ShaderManager_FromNodes : public ShaderManager {
public:
    ShaderManager_FromNodes();

    void update() override{};
    void imgui_window() override;

private:
    EdNodeEditor _node_editor;
    NodeManager  _node_manager;

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
    }
};
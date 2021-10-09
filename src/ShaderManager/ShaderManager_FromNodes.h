#pragma once

#include "ShaderManager.h"
#include "is0/NodeEditor.h"

class ShaderManager_FromNodes : public ShaderManager {
public:
    ShaderManager_FromNodes();

    void update() override{};
    void imgui_window() override;

private:
    NodeEditor _node_editor;

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
    }
};
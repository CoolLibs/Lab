#pragma once
#include "Cool/Nodes/Link.h"
#include "Node.h"

namespace Lab {

struct NodeInClipboard {
    NodeAsPOD node_as_pod{};
    ImVec2    position{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Node", node_as_pod),
            cereal::make_nvp("Position", position)
        );
    }
};

struct NodesClipboard {
    std::vector<NodeInClipboard> nodes;
    std::vector<Cool::Link>      links;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Nodes", nodes),
            cereal::make_nvp("Links", links)
        );
    }
};

} // namespace Lab
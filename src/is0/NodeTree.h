#pragma once

#include "Link.h"
#include "Node.h"

struct NodeTree {
    std::vector<Node> nodes;
    std::vector<Link> links;

    void add_node(Node node);
    void add_link(Link link);

    void delete_node(NodeId node_id);
    void delete_link(LinkId link_id);
    void delete_link_going_to(PinId pin_id);

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Nodes", nodes),
                cereal::make_nvp("Links", links));
    }
};
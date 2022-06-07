#pragma once

#include "Link.h"
#include "Node.h"

struct NodeTree {
    std::vector<Node> nodes;
    std::vector<Link> links;

    void add_node(const Node& node);
    void add_link(Link link);

    void delete_node(NodeId node_id);
    void delete_link(LinkId link_id);
    void delete_link_going_to(PinId pin_id);

    /**
     * @brief Assumes that pin is an input pin and returns the node that is connected to it (or nullptr if there is none)
     */
    const Node* find_input_node(const Pin& pin) const;
    const Node& find_node_with_output_pin(PinId pin_id) const;
    const Pin&  find_pin(PinId id);
    bool        has_no_successor(const Node& node) const;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        try {
            archive(cereal::make_nvp("Nodes", nodes),
                    cereal::make_nvp("Links", links));
        }
        catch (const std::exception&) {
            nodes.clear();
            links.clear();
            throw;
        }
    }
};
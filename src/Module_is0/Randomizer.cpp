#include "Randomizer.h"
#include <Cool/Random/Random.h>

static const NodeTemplate& random_node_template(const NodeFactory& factory)
{
    return factory.templates()[Cool::Random::size_type(0, factory.templates().size() - 1)];
}

static const NodeTemplate& random_node_template(const NodeFactory& factory, std::string_view folder)
{
    const auto range = factory.templates_from_folder(folder);
    if (range)
    {
        return factory.templates()[Cool::Random::size_type(
            range->first,
            range->last
        )];
    }
    else
    {
        return factory.templates()[0];
    }
}

NodeTree merge(const NodeTree& a, const NodeTree& b)
{
    auto res = a;
    res.nodes.insert(res.nodes.end(), b.nodes.begin(), b.nodes.end());
    res.links.insert(res.links.end(), b.links.begin(), b.links.end());
    return res;
}

static NodeTree random_node_tree_impl(const NodeFactory& factory, int depth, int max_depth)
{
    auto tree = NodeTree{};
    if (depth <= 0)
    {
        tree.nodes.push_back(NodeFactoryU::node_from_template(
            random_node_template(factory, "Shapes")
        ));
    }
    else
    {
        const auto node = NodeFactoryU::node_from_template(
            depth == max_depth
                ? random_node_template(factory, "Booleans")
                : random_node_template(factory)
        );
        tree.add_node(node);
        for (const auto& pin : node.input_pins)
        {
            const auto subtree = random_node_tree_impl(factory, depth - 1, max_depth);
            if (!subtree.nodes.empty())
            {
                tree.add_link({{}, subtree.nodes[0].output_pin.id(), pin.id()});
                tree = merge(tree, subtree);
            }
        }
    }
    return tree;
}

NodeTree random_node_tree(const NodeFactory& factory, int max_depth)
{
    return random_node_tree_impl(factory, max_depth, max_depth);
}
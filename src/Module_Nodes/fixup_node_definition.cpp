#include "fixup_node_definition.h"
#include <Module_Nodes/NodeDefinition.h>

namespace Lab {

/// If an RGBA goes into an RGB->RGB node, and then an RGBA pin,
/// we need to preserve the alpha during the RGB->RGB node.
/// This is why we implicitly turn it into an RGBA->RGBA node.
/// (NB: this also applies to RGB->RGBA nodes and the like.
/// For example "Color Ramp" is a Greyscale->RGBA node that we want to convert into a GreyscaleAlpha->RGBA)
static void add_alpha_forwarding(MainFunctionPieces& main)
{
    // if (!is_color_or_greyscale_modifier(main.signature))
    //     return;
}

void fixup_node_definition(NodeDefinition_Data& data)
{
    add_alpha_forwarding(data.main_function);
}

} // namespace Lab
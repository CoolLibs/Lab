#include "fixup_node_definition.h"
#include "FunctionSignature.h"
#include "NodeDefinition.h"
#include "PrimitiveType.h"
#include "valid_glsl.h"

namespace Lab {

/// If an RGBA goes into an RGB->RGB node, and then an RGBA pin,
/// we need to preserve the alpha during the RGB->RGB node.
/// This is why we implicitly turn it into an RGBA->RGBA node.
/// (NB: this also applies to RGB->RGBA nodes and the like.
/// For example "Color Ramp" is a Greyscale->RGBA node that we want to convert into a GreyscaleAlpha->RGBA)
static void add_alpha_forwarding(NodeDefinition_Data& data)
{
    using fmt::literals::operator""_a;

    if (!( // NOLINT(readability-simplify-boolean-expr)
            is_color_or_greyscale_modifier(data.main_function.signature)
            && !has_an_alpha_channel(data.main_function.signature.from)
        ))
    {
        return;
    }
    // /!\ NB: the following code assumes that the main function has exactly 1 parameter.

    // Use "main" as a helper function
    // TODO(NodesParsing)
    auto const name_of_original_main = valid_glsl(data.main_function.name() + "Original");
    // data.helper_functions.push_back(data.main_function.function);
    // data.helper_functions.back().name() = name_of_original_main;
    // Redefine "main"
    auto const original_output_type   = data.main_function.signature.to;
    data.main_function.signature.from = with_straight_alpha(data.main_function.signature.from);
    data.main_function.signature.to   = with_straight_alpha_if_has_no_alpha(data.main_function.signature.to);
    const char* color_components      = is_greyscale_type(data.main_function.signature.from) ? "x" : "rgb";
    const char* alpha_component       = is_greyscale_type(data.main_function.signature.from) ? "y" : "a";
    auto const  func_to_apply_alpha   = fmt::format(
        "Cool_apply_{}_alpha_to_{}{}",
        has_straight_alpha_channel(data.main_function.signature.to) ? "straight" : "premultiplied",
        is_greyscale_type(data.main_function.signature.to) ? "greyscale" : "color",
        has_an_alpha_channel(original_output_type) ? "_and_alpha" : ""
    );
    data.main_function.body =
        fmt::format(
            FMT_COMPILE("return {func_to_apply_alpha}({original_main}({color}.{color_components}), {color}.{alpha_component});"),
            "func_to_apply_alpha"_a = func_to_apply_alpha,
            "original_main"_a       = name_of_original_main,
            "color"_a               = data.main_function.argument_names[0],
            "color_components"_a    = color_components,
            "alpha_component"_a     = alpha_component
        );
}

void fixup_node_definition(NodeDefinition_Data& data)
{
    add_alpha_forwarding(data);
}

} // namespace Lab
#include "fixup_node_definition.h"
#include <Module_Nodes/FunctionSignature.h>
#include <Module_Nodes/NodeDefinition.h>
#include <Module_Nodes/PrimitiveType.h>
#include "helpers.h"

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
            is_color_or_greyscale_modifier(data.main_function.signature.signature)
            && !has_an_alpha_channel(data.main_function.signature.signature.from)
        ))
    {
        return;
    }
    // /!\ NB: the following code assumes that the main function has exactly 1 parameter.

    // Use "main" as a helper function
    auto const name_of_original_main = valid_glsl(data.main_function.name + "Original");
    data.helper_functions.push_back(FunctionPieces{
        .name      = name_of_original_main,
        .signature = CompleteFunctionSignature{
            .parameters = [&] { // HACK: wrapped in an immediately-invoked lambda to work around a bug in MSVC :/
                return ParametersList{ParamDesc{
                    .name = data.main_function.signature.parameter_names[0],
                    .type = data.main_function.signature.signature.from,
                }};
            }(),
            .output_type = data.main_function.signature.signature.to,
        },
        .body = data.main_function.body,
    });
    // Redefine "main"
    auto const original_output_type             = data.main_function.signature.signature.to;
    data.main_function.signature.signature.from = with_straight_alpha(data.main_function.signature.signature.from);
    data.main_function.signature.signature.to   = with_straight_alpha_if_has_no_alpha(data.main_function.signature.signature.to);
    const char* color_components                = is_greyscale_type(data.main_function.signature.signature.from) ? "x" : "rgb";
    const char* alpha_component                 = is_greyscale_type(data.main_function.signature.signature.from) ? "y" : "a";
    auto const  func_to_apply_alpha             = fmt::format(
        "Cool_apply_{}_alpha_to_{}{}",
        has_straight_alpha_channel(data.main_function.signature.signature.to) ? "straight" : "premultiplied",
        is_greyscale_type(data.main_function.signature.signature.to) ? "greyscale" : "color",
        has_an_alpha_channel(original_output_type) ? "_and_alpha" : ""
    );
    data.main_function.body =
        fmt::format(
            FMT_COMPILE("return {func_to_apply_alpha}({original_main}({color}.{color_components}), {color}.{alpha_component});"),
            "func_to_apply_alpha"_a = func_to_apply_alpha,
            "original_main"_a       = name_of_original_main,
            "color"_a               = data.main_function.signature.parameter_names[0],
            "color_components"_a    = color_components,
            "alpha_component"_a     = alpha_component
        );
}

void fixup_node_definition(NodeDefinition_Data& data)
{
    add_alpha_forwarding(data);
}

} // namespace Lab
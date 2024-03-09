#include "fixup_node_definition.h"
#include "FunctionSignature.h"
#include "NodeDefinition.h"
#include "PrimitiveType.h"
#include "gen_function_definition.h"
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
    auto const name_of_original_main = valid_glsl(data.main_function.name() + "Original");
    data.helper_glsl_code += gen_function_definition({
        .signature_as_string = data.main_function.signature_as_string,
        .unique_name         = name_of_original_main,
        .before_function     = "",
        .body                = data.main_function.body,
    });
    data.names_in_global_scope.push_back(name_of_original_main);
    // Redefine "main"
    auto const original_output_type        = data.main_function.signature.to;
    data.main_function.signature.from      = with_straight_alpha(data.main_function.signature.from);
    data.main_function.signature.to        = with_straight_alpha_if_has_no_alpha(data.main_function.signature.to);
    data.main_function.signature_as_string = FunctionSignatureAsString{
        .return_type    = glsl_type_as_string(data.main_function.signature.to),
        .name           = data.main_function.signature_as_string.name,
        .arguments_list = fmt::format("{} Color", glsl_type_as_string(data.main_function.signature.from)),
    };
    data.main_function.body =
        fmt::format(
            FMT_COMPILE("return {func_to_apply_alpha}({original_main}(Color.{color_components}), Color.{alpha_component});"),
            "func_to_apply_alpha"_a = fmt::format(
                "Cool_apply_{}_alpha_to_{}{}",
                has_straight_alpha_channel(data.main_function.signature.to) ? "straight" : "premultiplied",
                is_greyscale_type(data.main_function.signature.to) ? "greyscale" : "color",
                has_an_alpha_channel(original_output_type) ? "_and_alpha" : ""
            ),
            "original_main"_a    = name_of_original_main,
            "color"_a            = data.main_function.argument_names[0],
            "color_components"_a = is_greyscale_type(data.main_function.signature.from) ? "x" : "rgb",
            "alpha_component"_a  = is_greyscale_type(data.main_function.signature.from) ? "y" : "a"
        );
}

void fixup_node_definition(NodeDefinition_Data& data)
{
    add_alpha_forwarding(data);
}

} // namespace Lab
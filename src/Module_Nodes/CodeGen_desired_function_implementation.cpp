#include "CodeGen_desired_function_implementation.h"
#include <Module_Nodes/CodeGen.h>
#include <Module_Nodes/FunctionSignature.h>
#include <string>
#include "CodeGen.h"
#include "CodeGen_default_function.h"
#include "CodeGen_implicit_conversion.h"
#include "Module_Nodes/CodeGenContext.h"
#include "Module_Nodes/CodeGen_implicit_conversion.h"
#include "Module_Nodes/FunctionSignature.h"
#include "Module_Nodes/PrimitiveType.h"
#include "tl/expected.hpp"

namespace Lab {

namespace {

class TransformationStrategy_DoNothing {
public:
    static auto gen_func(Cool::InputPin const&, CodeGenContext&)
        -> ExpectedFunctionName
    {
        return "";
    }
};

class TransformationStrategy_UseDefaultFunction {
public:
    auto gen_func(Cool::InputPin const&, CodeGenContext& context) const
        -> ExpectedFunctionName
    {
        return gen_default_function(
            _signature,
            context
        );
    }

    explicit TransformationStrategy_UseDefaultFunction(FunctionSignature signature)
        : _signature{signature} {}

private:
    FunctionSignature _signature;
};

class TransformationStrategy_UseInputNode {
public:
    auto gen_func(Cool::InputPin const& pin, CodeGenContext& context) const
        -> ExpectedFunctionName
    {
        return gen_desired_function(
            _signature,
            pin,
            context
        );
    }

    explicit TransformationStrategy_UseInputNode(FunctionSignature signature)
        : _signature{signature} {}

private:
    FunctionSignature _signature;
};

class TransformationStrategy_UseInputNodeIfItExists {
public:
    auto gen_func(Cool::InputPin const& pin, CodeGenContext& context) const
        -> ExpectedFunctionName
    {
        return gen_desired_function(
            _signature,
            pin,
            context,
            false /*fallback_to_a_default_function*/ // The default behavior of gen_desired_function() when there is no input node is to try to generate a default function, which is not what we want in the case of this strategy. This is what differentiates it from TransformationStrategy_UseInputNode
        );
    }

    explicit TransformationStrategy_UseInputNodeIfItExists(FunctionSignature signature)
        : _signature{signature} {}

private:
    FunctionSignature _signature;
};

class TransformationStrategy {
public:
    using TransformationStrategyVariant = std::variant<
        TransformationStrategy_DoNothing,
        TransformationStrategy_UseDefaultFunction,
        TransformationStrategy_UseInputNode,
        TransformationStrategy_UseInputNodeIfItExists>;

    TransformationStrategy(TransformationStrategyVariant strategy) // NOLINT (google-explicit-constructor)
        : _strategy{strategy}
    {}

    auto gen_func(Cool::InputPin const& pin, CodeGenContext& context) const
        -> ExpectedFunctionName
    {
        return std::visit([&](auto&& strategy) { return strategy.gen_func(pin, context); }, _strategy);
    }

private:
    TransformationStrategyVariant _strategy;
};

} // namespace

static auto input_transformation(
    FunctionSignature          current,
    FunctionSignature          desired,
    ImplicitConversions const& implicit_conversions
) -> TransformationStrategy
{
    auto const signature = FunctionSignature{
        .from  = desired.from,
        .to    = current.from,
        .arity = desired.from != PrimitiveType::Void ? 1u : 0u,
    };

    if (!implicit_conversions.input) // Input needs to be converted in order to have the right type
        return {TransformationStrategy_UseInputNode{signature}};

    if (implicit_conversions.output) // Input pin is not used to convert output, so we will still apply it to our input so that it is used somewhere
        return {TransformationStrategy_UseInputNodeIfItExists{signature}};

    // Input pin is used elsewhere and we don't need it here, so use nothing
    return {TransformationStrategy_DoNothing{}};
}

static auto output_transformation(
    FunctionSignature          current,
    FunctionSignature          desired,
    ImplicitConversions const& implicit_conversions
) -> TransformationStrategy
{
    if (implicit_conversions.output)
        // We don't need to convert the output
        return {TransformationStrategy_DoNothing{}};

    auto const signature = FunctionSignature{
        .from  = current.to,
        .to    = desired.to,
        .arity = current.to != PrimitiveType::Void ? 1u : 0u,
    };

    if (!implicit_conversions.input)
        // Use a default function because the input pin is already used to convert the inputs
        return {TransformationStrategy_UseDefaultFunction{signature}};

    // Use the input pin, it is not used to transform the inputs
    return {TransformationStrategy_UseInputNode{signature}};
}

static auto argument_name(size_t i, size_t desired_arity)
    -> std::string
{
    if (desired_arity == 0)
        return "";

    return fmt::format("in{}", std::min(i + 1u, desired_arity));
}

static auto gen_transformed_inputs(std::vector<std::string> const& transforms_names, size_t current_arity, size_t desired_arity, std::string const& implicit_conversion) -> std::string
{
    assert(transforms_names.size() == current_arity);

    std::string res{};

    for (size_t i = 0; i < current_arity; ++i)
    {
        res += fmt::format("{}({}({}))", transforms_names[i], implicit_conversion, argument_name(i, desired_arity));
        if (i != current_arity - 1)
            res += ", ";
    }

    return res;
}

static auto has_an_alpha_channel(PrimitiveType type) -> bool
{
    switch (type)
    {
#include "generated/has_an_alpha_channel.inl"
    default:
        return false;
    }
}

static auto gen_implicit_curve_renderer(
    FunctionSignature   desired,
    std::string_view    base_function_name,
    Node const&         node,
    Cool::NodeId const& node_id,
    CodeGenContext&     context
) -> tl::expected<std::string, std::string>
{
    auto const curve_func_name = gen_desired_function(curve_signature(), node, node_id, context);
    if (!curve_func_name)
        return tl::make_unexpected(curve_func_name.error());
    auto const shape_func_name = fmt::format("curveRenderer{}", valid_glsl(std::string{base_function_name}));
    context.push_function(Function{
        .name           = shape_func_name,
        .implementation = fmt::format(R"STR(
float {}/*coollabdef*/(vec2 uv)
{{
    const int NB_SEGMENTS = 300;
    const float THICKNESS = 0.01;

    float dist_to_curve = FLT_MAX;
    vec2  previous_position; // Will be filled during the first iteration of the loop

    for (int i = 0; i <= NB_SEGMENTS; i++)
    {{
        float t = i / float(NB_SEGMENTS); // 0 to 1

        vec2 current_position = {}(t);
        if (i != 0) // During the first iteration we don't yet have two points to draw a segment between
        {{
            float segment = Coollab_sdSegment(uv, previous_position, current_position, THICKNESS);
            dist_to_curve = min(dist_to_curve, segment);
        }}

        previous_position = current_position;
    }}

    return dist_to_curve;
}}
)STR",
                                      shape_func_name, *curve_func_name),
    });
    return gen_desired_function_implementation(shape_2D_signature(), desired, shape_func_name, node, node_id, context);
}

static auto gen_implicit_shape_3D_renderer(
    FunctionSignature   desired,
    std::string_view    base_function_name,
    Node const&         node,
    Cool::NodeId const& node_id,
    CodeGenContext&     context
) -> tl::expected<std::string, std::string>
{
    using fmt::literals::operator""_a;

    auto const shape_3D_func_name = gen_desired_function(shape_3D_signature(), node, node_id, context);
    if (!shape_3D_func_name)
        return tl::make_unexpected(shape_3D_func_name.error());
    auto const image_func_name = fmt::format("shape3DRenderer{}", valid_glsl(std::string{base_function_name}));
    context.push_function(Function{
        .name           = image_func_name,
        .implementation = fmt::format(
            FMT_COMPILE(R"STR(
vec4 {image_name}/*coollabdef*/(vec2 uv)
{{
    const int MAX_STEPS = 100;
    const float MAX_DIST = 100.;
    const float SURF_DIST = .001;

    vec3 ro = cool_ray_origin(uv);
    vec3 rd = cool_ray_direction(uv);

    // Ray march
    float d = 0.;
    for (int i = 0; i < MAX_STEPS; i++)
    {{
        vec3  p  = ro + rd * d;
        float dS = {shape_3D}(p);
        d += dS;
        if (d > MAX_DIST || abs(dS) < SURF_DIST)
            break;
    }}

    // Background
    if (d >= MAX_DIST)
        return vec4(0.);
        
    // Return the normal as a color 
    vec3 p = ro + rd * d;
    const vec2 e = vec2(.001, 0);
    vec3 normal = {shape_3D}(p)-vec3({shape_3D}(p - e.xyy), {shape_3D}(p - e.yxy), {shape_3D}(p - e.yyx));
    normal = normalize(normal) * 0.5 + 0.5;
    return vec4(normal, 1.);
}}
)STR"),
            "image_name"_a = image_func_name,
            "shape_3D"_a   = *shape_3D_func_name
        ),
    });
    auto const image_signature = FunctionSignature{.from = PrimitiveType::UV, .to = PrimitiveType::LinearRGB_StraightA, .arity = 1};
    return gen_desired_function_implementation(image_signature, desired, image_func_name, node, node_id, context);
}

auto gen_desired_function_implementation(
    FunctionSignature   current,
    FunctionSignature   desired,
    std::string_view    base_function_name,
    Node const&         node,
    Cool::NodeId const& node_id,
    CodeGenContext&     context
) -> tl::expected<std::string, std::string>
{
    using fmt::literals::operator""_a;

    if (is_curve(current) && !is_curve(desired))
        return gen_implicit_curve_renderer(desired, base_function_name, node, node_id, context);
    if (is_shape_3D(current) && !is_shape_3D(desired))
        return gen_implicit_shape_3D_renderer(desired, base_function_name, node, node_id, context);

    auto const implicit_conversions = gen_implicit_conversions(current, desired, context);

    auto input_transformation_names = std::vector<std::string>{};
    input_transformation_names.reserve(current.arity);
    for (size_t i = 0; i < current.arity; ++i)
    {
        auto const input_transformation_name = input_transformation(current, desired, implicit_conversions)
                                                   .gen_func(node.number_of_main_input_pins() > i ? node.main_input_pin(i) : Cool::InputPin{}, context);
        if (!input_transformation_name)
            return input_transformation_name;

        input_transformation_names.push_back(*input_transformation_name);
    }

    auto const output_transformation_name = output_transformation(current, desired, implicit_conversions)
                                                .gen_func(node.number_of_main_input_pins() > 0 ? node.main_input_pin(0) : Cool::InputPin{}, context);
    if (!output_transformation_name)
        return output_transformation_name;

    auto const call_base_function = fmt::format(
        FMT_COMPILE("{base_function}({inputs})"),
        "base_function"_a = base_function_name,
        "inputs"_a        = gen_transformed_inputs(input_transformation_names, current.arity, desired.arity, implicit_conversions.input.value_or(""))
    );

    auto const does_output_uv = current.to == PrimitiveType::UV
                                || (current.to == PrimitiveType::Vec2 && desired.to == PrimitiveType::UV); // In case we want to output UV and implicitly convert from vec2, we want that transformation to apply to our global UV.
    auto const does_output_alpha = has_an_alpha_channel(current.to);
    return fmt::format(
        FMT_COMPILE(R"STR(
{store_uv}
{store_alpha}
return {transform_output}({implicit_output_conversion}({base_function_output}));
)STR"),
        "store_uv"_a                   = does_output_uv ? fmt::format("coollab_context.uv = {};", call_base_function) : "",
        "store_alpha"_a                = does_output_alpha ? fmt::format("vec4 color = {}; coollab_global_alpha = color.a;", call_base_function) : "",
        "base_function_output"_a       = does_output_uv      ? "coollab_context.uv"
                                         : does_output_alpha ? "color"
                                                             : call_base_function,
        "transform_output"_a           = *output_transformation_name,
        "implicit_output_conversion"_a = implicit_conversions.output.value_or("")
    );
}

} // namespace Lab
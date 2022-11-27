#include "CodeGen_desired_function_implementation.h"
#include "CodeGen.h"
#include "CodeGen_default_function.h"
#include "Module_Nodes/PrimitiveType.h"

// TODO(JF) test all of these, to make sure overload resolution doesn't change when we add options

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
            context.graph().input_node_id(pin.id()),
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
        auto const node_id = context.graph().input_node_id(pin.id());

        if (!context.graph().nodes().contains(node_id))
            return ""; // The default behaviour of gen_desired_function() when there is no input node is to try to generate a default function, which is not what we want in the case of this strategy. This is what differentiates it from TransformationStrategy_UseInputNode

        return gen_desired_function(
            _signature,
            node_id,
            context
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
    FunctionSignature current,
    FunctionSignature desired
) -> TransformationStrategy
{
    auto const signature = FunctionSignature{
        .from  = desired.from,
        .to    = current.from,
        .arity = desired.from != PrimitiveType::Void ? 1u : 0u,
    };

    if (current.from != desired.from) // Input needs to be converted in order to have the right type
        return {TransformationStrategy_UseInputNode{signature}};

    if (current.to == desired.to) // Input pin is not used to convert output, so we will still apply it to our input so that it is used somewhere
        return {TransformationStrategy_UseInputNodeIfItExists{signature}};

    // Input pin is used elsewhere and we don't need it here, so use nothing
    return {TransformationStrategy_DoNothing{}};
}

static auto output_transformation(
    FunctionSignature current,
    FunctionSignature desired
) -> TransformationStrategy
{
    if (current.to == desired.to)
        // We don't need to convert the output
        return {TransformationStrategy_DoNothing{}};

    auto const signature = FunctionSignature{
        .from  = current.to,
        .to    = desired.to,
        .arity = current.to != PrimitiveType::Void ? 1u : 0u,
    };

    if (current.from != desired.from)
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

static auto gen_transformed_inputs(std::vector<std::string> const& transforms_names, size_t current_arity, size_t desired_arity) -> std::string
{
    assert(transforms_names.size() == current_arity);

    std::string res{};

    for (size_t i = 0; i < current_arity; ++i)
    {
        res += fmt::format("{}({})", transforms_names[i], argument_name(i, desired_arity));
        if (i != current_arity - 1)
            res += ", ";
    }

    return res;
}

auto gen_desired_function_implementation(
    FunctionSignature current,
    FunctionSignature desired,
    std::string_view  base_function_name,
    Node const&       node,
    CodeGenContext&   context

) -> tl::expected<std::string, std::string>
{
    using fmt::literals::operator""_a;

    auto input_transformation_names = std::vector<std::string>{};
    input_transformation_names.reserve(current.arity);
    for (size_t i = 0; i < current.arity; ++i)
    {
        auto const input_transformation_name = input_transformation(current, desired)
                                                   .gen_func(node.main_input_pin(i), context);
        if (!input_transformation_name)
            return input_transformation_name;

        input_transformation_names.push_back(*input_transformation_name);
    }

    auto const output_transformation_name = output_transformation(current, desired)
                                                .gen_func(current.arity > 0 ? node.main_input_pin(0) : Cool::InputPin{}, context);
    if (!output_transformation_name)
        return output_transformation_name;

    return fmt::format(
        FMT_COMPILE(
            "return {transform_output}({base_function}({transformed_inputs}));"
        ),
        "base_function"_a      = base_function_name,
        "transformed_inputs"_a = gen_transformed_inputs(input_transformation_names, current.arity, desired.arity),
        "transform_output"_a   = *output_transformation_name
    );
}

} // namespace Lab
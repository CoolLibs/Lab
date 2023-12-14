#pragma once
#include <Cool/Nodes/NodeId.h>
#include <Nodes/NodeDefinition.h>
#include "CodeGenContext.h"
#include "FunctionSignature.h"
#include "MaybeGenerateModule.h"
#include "Node.h"


namespace Lab {

auto make_valid_output_index_name(Cool::OutputPin const& pin)
    -> std::string;

/// Creates a function with the `desired_signature` from the node connected to `pin`,
/// or a default function if nothing is connected to `pin` and `fallback_to_a_default_function` is true.
auto gen_desired_function(
    FunctionSignature          desired_signature,
    Cool::InputPin const&      pin,
    CodeGenContext&            context,
    MaybeGenerateModule const& maybe_generate_module,
    bool                       fallback_to_a_default_function = true
) -> ExpectedFunctionName;

/// Creates a function with the `desired_signature` from the node with the given id,
/// or a default function if the `id` is invalid and `fallback_to_a_default_function` is true.
auto gen_desired_function(
    FunctionSignature          desired_signature,
    Cool::NodeId const&        id,
    CodeGenContext&            context,
    MaybeGenerateModule const& maybe_generate_module,
    bool                       fallback_to_a_default_function = true
) -> ExpectedFunctionName;

/// Creates a function with the `desired_signature` from the given `maybe_node`,
/// or a default function if `maybe_node` is nullopt and `fallback_to_a_default_function` is true.
auto gen_desired_function(
    FunctionSignature          desired_signature,
    Node const*                maybe_node,
    Cool::NodeId const&        id,
    CodeGenContext&            context,
    MaybeGenerateModule const& maybe_generate_module,
    bool                       fallback_to_a_default_function = true
) -> ExpectedFunctionName;

/// Creates a function with the `desired_signature` from the given `node`.
auto gen_desired_function(
    FunctionSignature                  desired_signature,
    std::reference_wrapper<Node const> node,
    Cool::NodeId const&                id,
    CodeGenContext&                    context,
    MaybeGenerateModule const&         maybe_generate_module
) -> ExpectedFunctionName;

} // namespace Lab

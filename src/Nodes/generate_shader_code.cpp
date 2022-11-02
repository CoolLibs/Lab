#include "generate_shader_code.h"
#include <fmt/compile.h>
#include "code_gen.h"

namespace Lab {

auto generate_shader_code(
    Graph const&        graph,
    NodesLibrary const& nodes_library,
    Cool::NodeId const& main_node_id
) -> std::string
{
    const auto main_node = graph.nodes().get(main_node_id);
    if (!main_node)
        return "";

    const auto main_function_definition = gen_desired_function(
        *main_node,
        main_node_id,
        Signature::Image,
        "main_function",
        "prout",
        Cool::GetNodeDefinition_Ref{nodes_library}
    );
    if (!main_function_definition)
        return "";

    using namespace fmt::literals;
    return fmt::format(
        FMT_COMPILE(R"STR(

{main_function_definition}

void main()
{{
    vec2 uv = normalized_uv();
    gl_FragColor = vec4(main_function(uv), 1.);
}}

)STR"
        ),
        "main_function_definition"_a = main_function_definition->data
    );
}

} // namespace Lab
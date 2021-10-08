using namespace Cool;

#include "NodeEditor.h"
#include "Node.h"
#include "NodeFactory.h"

NodeEditor::NodeEditor()
    : _context(ed::CreateEditor())
{
}

NodeEditor::~NodeEditor()
{
    ed::DestroyEditor(_context);
}

void NodeEditor::subscribe_to_tree_change(std::function<void(NodeEditor&)> callback)
{
    _on_tree_change_callbacks.push_back(callback);
    callback(*this);
}

void NodeEditor::on_tree_change()
{
    for (auto& callback : _on_tree_change_callbacks)
        callback(*this);
}

std::string NodeEditor::gen_scene_sdf()
{
    std::string s;
    bool        b = false;
    // TODO improve me with a group
    (_registry.view<IsTerminalNode>() | _registry.view<Node>()).each([&](auto, Node& node) {
        s += "d = min(d, " + node.fn_name + "(pos));\n";
        b = true;
    });
    if (b)
        return "float sceneSDF(vec3 pos) {\n"
               "float d = MAX_DIST;\n" +
               s +
               "return d;\n"
               "}";
    else
        return "float sceneSDF(vec3 pos) {"
               "return length(pos) - 10.;"
               "}";
}

Cool::ShaderSource NodeEditor::gen_raymarching_shader_code()
{
    // Update the source codes
    _registry.view<Node>().each([&](auto e, Node& node) {
        node.fn_implementation = NodeFactory::fn_implementation(NodeFactory::fn_signature(node.fn_name), node.gen_source_code(e));
    });

    // Collect all function declarations and implementations
    std::string function_declarations    = "";
    std::string function_implementations = "";

    _registry.view<Node>().each([&](auto, Node& node) {
        function_declarations += node.fn_declaration;
        function_implementations += node.fn_implementation;
    });

    //
    return ShaderSource{R"V0G0N(
#version 430

in vec2       vTexCoords;
uniform float _time;
#include "Cool/res/shaders/camera.glsl"

// ----- Ray marching options ----- //
#define MAX_STEPS 150
#define MAX_DIST 200.
#define SURF_DIST 0.0001
#define NORMAL_DELTA 0.0001

#define saturate(v) clamp(v, 0., 1.)
)V0G0N" + function_declarations +
                        function_implementations + gen_scene_sdf() + R"V0G0N(

float rayMarching(vec3 ro, vec3 rd) {
    float t = 0.;
 	
    for (int i = 0; i < MAX_STEPS; i++) {
    	vec3 pos = ro + rd * t;
        float d = sceneSDF(pos);
        t += d;
        // If we are very close to the object, consider it as a hit and exit this loop
        if( t > MAX_DIST || abs(d) < SURF_DIST*0.99) break;
    }
    return t;
}

vec3 getNormal(vec3 p) {
    const float h = NORMAL_DELTA;
	const vec2 k = vec2(1., -1.);
    return normalize( k.xyy * sceneSDF( p + k.xyy*h ) + 
                      k.yyx * sceneSDF( p + k.yyx*h ) + 
                      k.yxy * sceneSDF( p + k.yxy*h ) + 
                      k.xxx * sceneSDF( p + k.xxx*h ) );
}

vec3 render(vec3 ro, vec3 rd) {
    vec3 finalCol = vec3(0.3, 0.7, 0.98);
    
    float d = rayMarching(ro, rd);
    
    if (d < MAX_DIST) {
      vec3 p = ro + rd * d;
      vec3 normal = getNormal(p); 
      //vec3 ref = reflect(rd, normal);
      
      //float sunFactor = saturate(dot(normal, nSunDir));
      //float sunSpecular = pow(saturate(dot(nSunDir, ref)), specularStrength); // Phong
    
      finalCol = normal * 0.5 + 0.5;
    }
    
    finalCol = saturate(finalCol);
    finalCol = pow(finalCol, vec3(0.4545)); // Gamma correction
    return finalCol;
}

void main() {
    vec3 ro = cool_ray_origin();
    vec3 rd = cool_ray_direction();
    gl_FragColor = vec4(render(ro, rd), 1.);
}
)V0G0N"};
}

PinInfo NodeEditor::compute_pin_infos(ed::PinId pin_id)
{
    PinInfo pin_info;
    _registry.view<ShapeNode>().each([&](auto e, ShapeNode& shape_node) {
        if (shape_node.output_pin.id == pin_id) {
            pin_info.kind        = ed::PinKind::Output;
            pin_info.node_entity = e;
        }
    });
    _registry.view<ModifierNode>().each([&](auto e, ModifierNode& modifier_node) {
        if (modifier_node.output_pin.id == pin_id) {
            pin_info.kind        = ed::PinKind::Output;
            pin_info.node_entity = e;
        }
        if (modifier_node.input_pin.id == pin_id) {
            pin_info.kind        = ed::PinKind::Input;
            pin_info.node_entity = e;
        }
    });
    assert(_registry.valid(pin_info.node_entity));
    return pin_info;
}

entt::entity NodeEditor::compute_node_connected_to_pin(ed::PinId pin_id)
{
    for (const auto& link : _links) {
        if (link.start_pin_id == pin_id) {
            return link.end_node_entity;
        }
        if (link.end_pin_id == pin_id) {
            return link.start_node_entity;
        }
    }
    return entt::null;
}

void NodeEditor::ImGui_window()
{
    ImGui::Begin("Nodes");
    ed::SetCurrentEditor(_context);
    ed::Begin("My Editor");

    //
    // 1) Commit known data to editor
    //

    // Draw Shape Nodes
    _registry.view<ShapeNode>().each([&](auto e, ShapeNode& shape_node) {
        const Node& node = _registry.get<Node>(e);
        ed::BeginNode(node.node_id);
        ImGui::Text("%s", node.name.c_str());
        ImGui::BeginGroup();
        if (node.show_widgets(e)) {
            on_tree_change();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ed::BeginPin(shape_node.output_pin.id, ed::PinKind::Output);
        ImGui::Text("OUT->");
        ed::EndPin();
        ed::EndNode();
    });
    // Draw Modifier Nodes
    _registry.view<ModifierNode>().each([&](auto e, ModifierNode& modifier_node) {
        const Node& node = _registry.get<Node>(e);
        ed::BeginNode(node.node_id);
        ImGui::Text("%s", node.name.c_str());
        ed::BeginPin(modifier_node.input_pin.id, ed::PinKind::Input);
        ImGui::Text("->IN");
        ed::EndPin();
        ImGui::SameLine();
        ImGui::BeginGroup();
        if (node.show_widgets(e)) {
            on_tree_change();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ed::BeginPin(modifier_node.output_pin.id, ed::PinKind::Output);
        ImGui::Text("OUT->");
        ed::EndPin();
        ed::EndNode();
    });

    // Submit Links
    for (auto& linkInfo : _links)
        ed::Link(linkInfo.id, linkInfo.start_pin_id, linkInfo.end_pin_id);

    //
    // 2) Handle interactions
    //

    // Handle creation action, returns true if editor want to create new object (node or link)
    if (ed::BeginCreate()) {
        ed::PinId start_pin_id, end_pin_id;
        if (ed::QueryNewLink(&start_pin_id, &end_pin_id)) {
            if (start_pin_id && end_pin_id && ed::AcceptNewItem()) {
                PinInfo start_pin_info = compute_pin_infos(start_pin_id);
                PinInfo end_pin_info   = compute_pin_infos(end_pin_id);

                if (start_pin_info.kind == ed::PinKind::Input) // Reorder so that we always go from an output pin to an input pin
                {
                    std::swap(start_pin_info, end_pin_info);
                    std::swap(start_pin_id, end_pin_id);
                }

                bool accept_link = true;
                // Check that there isn't already a node connected to the end_pin
                accept_link &= !_registry.valid(compute_node_connected_to_pin(end_pin_id));
                // Check that one pin is an input and the other an output
                accept_link &= start_pin_info.kind != end_pin_info.kind;
                // Check that we are not linking a node to itself
                accept_link &= start_pin_info.node_entity != end_pin_info.node_entity;

                if (accept_link) {
                    _links.push_back({ed::LinkId(NodeFactory::NextId()), start_pin_id, start_pin_info.node_entity, end_pin_id, end_pin_info.node_entity});
                    _registry.remove_if_exists<IsTerminalNode>(start_pin_info.node_entity);
                    on_tree_change();
                    // Draw new link
                    //ed::Link(_links.back().id, _links.back().start_pin_id, _links.back().end_pin_id);
                }
                else {
                    ed::RejectNewItem();
                }
            }
        }
    }
    ed::EndCreate(); // Wraps up object creation action handling.

    // Handle deletion action
    if (ed::BeginDelete()) {
        // There may be many links marked for deletion, let's loop over them.
        ed::LinkId deletedLinkId;
        while (ed::QueryDeletedLink(&deletedLinkId)) {
            // If you agree that link can be deleted, accept deletion.
            if (ed::AcceptDeletedItem()) {
                // Then remove link from your data.
                for (auto& link : _links) {
                    if (link.id == deletedLinkId) {
                        _registry.emplace<IsTerminalNode>(link.start_node_entity);
                        _links.erase(&link);
                        on_tree_change();
                        break;
                    }
                }
            }

            // You may reject link deletion by calling:
            // ed::RejectDeletedItem();
        }
    }
    ed::EndDelete(); // Wrap up deletion action
    static int coutn = 0;
    ed::End();
    if (ImGui::BeginPopupContextItem("sdfxaas", ImGuiPopupFlags_MouseButtonMiddle)) {
        if (ImGui::Button("Sphere")) {
            NodeFactory::sphere(_registry);
            on_tree_change();
        }
        if (ImGui::Button("Cube")) {
            NodeFactory::cube(_registry);
            on_tree_change();
        }
        if (ImGui::Button("Transform")) {
            NodeFactory::transform(_registry, *this);
            on_tree_change();
        }
        if (ImGui::Button("Repeat Infinite")) {
            NodeFactory::repeat_infinite(_registry, *this);
            on_tree_change();
        }
        if (ImGui::Button("Twist")) {
            NodeFactory::twist(_registry, *this);
            on_tree_change();
        }
        ImGui::EndPopup();
    }
    ImGui::End();
}
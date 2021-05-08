using namespace Cool;

#include "NodeEditor.h"
#include "Node_Sphere.h"

NodeEditor::NodeEditor()
	: _context(ed::CreateEditor())
{}

NodeEditor::~NodeEditor() {
	ed::DestroyEditor(_context);
}

void NodeEditor::subscribe_to_tree_change(std::function<void(NodeEditor&)> callback) {
    _on_tree_change_callbacks.push_back(callback);
    callback(*this);
}

void NodeEditor::on_tree_change() {
    for (auto& callback : _on_tree_change_callbacks)
        callback(*this);
}

std::string NodeEditor::gen_scene_sdf() {
    std::string s;
    for (const auto& node : _nodes) {
        s += "d = min(d, " + node->function_name() + "(pos));\n";
    }
    return 
    "float sceneSDF(vec3 pos) {\n"
        "float d = MAX_DIST;\n"
        + s +
        "return d;\n"
    "}\n";
}

std::string NodeEditor::gen_raymarching_shader_code() {
    std::string function_declarations = "";
    for (const auto& node : _nodes) {
        function_declarations += node->function_declaration();
    }

    std::string function_implementations = "";
    for (const auto& node : _nodes) {
        function_implementations += node->function_implementation();
    }

    return R"V0G0N(
#version 430

in vec2 vTexCoords;
uniform float uAspectRatio;
uniform vec3 uCamX;
uniform vec3 uCamY;
uniform vec3 uCamZ;
uniform vec3 uCamPos;
uniform float uFocalLength;
uniform float uTime;

// ----- Ray marching options ----- //
#define MAX_STEPS 150
#define MAX_DIST 200.
#define SURF_DIST 0.0001
#define NORMAL_DELTA 0.0001

#define saturate(v) clamp(v, 0., 1.)
)V0G0N"
+ function_declarations
+ function_implementations
+ gen_scene_sdf()
+ R"V0G0N(

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
    vec3 finalCol = vec3(1., 0., 1.);
    
    float d = rayMarching(ro, rd);
    
    if (d < MAX_DIST) {
      vec3 p = ro + rd * d;
      vec3 normal = getNormal(p); 
      //vec3 ref = reflect(rd, normal);
      
      //float sunFactor = saturate(dot(normal, nSunDir));
      //float sunSpecular = pow(saturate(dot(nSunDir, ref)), specularStrength); // Phong
    
      finalCol = normal;
    }
    
    finalCol = saturate(finalCol);
    finalCol = pow(finalCol, vec3(0.4545)); // Gamma correction
    return finalCol;
}

void main() {
    vec3 ro = uCamPos;
    vec3 rd = normalize(
              uCamX * (vTexCoords.x - 0.5) * uAspectRatio
            + uCamY * (vTexCoords.y - 0.5)
            - uCamZ * uFocalLength
    );
    gl_FragColor = vec4(render(ro, rd), 1.);
}
)V0G0N";
}

void NodeEditor::ImGui_window()
{
    ImGui::Begin("Nodes");
    ed::SetCurrentEditor(_context);
    ed::Begin("My Editor");
    int uniqueId = 1;

    //
    // 1) Commit known data to editor
    //

    // Submit nodes
    for (const auto& node : _nodes) {
        ed::BeginNode(uniqueId++);
        ImGui::Text(node->name().data());
        ed::BeginPin(uniqueId++, ed::PinKind::Input);
        ImGui::Text("->");
        ed::EndPin();
        ImGui::SameLine();
        ed::BeginPin(uniqueId++, ed::PinKind::Output);
        ImGui::Text("->");
        ed::EndPin();
        ed::EndNode();
    }

    // Submit Links
    for (auto& linkInfo : _links)
        ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

    //
    // 2) Handle interactions
    //

    // Handle creation action, returns true if editor want to create new object (node or link)
    if (ed::BeginCreate())
    {
        ed::PinId inputPinId, outputPinId;
        if (ed::QueryNewLink(&inputPinId, &outputPinId))
        {
            // QueryNewLink returns true if editor want to create new link between pins.
            //
            // Link can be created only for two valid pins, it is up to you to
            // validate if connection make sense. Editor is happy to make any.
            //
            // Link always goes from input to output. User may choose to drag
            // link from output pin or input pin. This determine which pin ids
            // are valid and which are not:
            //   * input valid, output invalid - user started to drag new ling from input pin
            //   * input invalid, output valid - user started to drag new ling from output pin
            //   * input valid, output valid   - user dragged link over other pin, can be validated

            if (inputPinId && outputPinId) // both are valid, let's accept link
            {
                // ed::AcceptNewItem() return true when user release mouse button.
                if (ed::AcceptNewItem())
                {
                    // Since we accepted new link, lets add one to our list of links.
                    _links.push_back({ ed::LinkId(_next_link_id++), inputPinId, outputPinId });

                    // Draw new link.
                    ed::Link(_links.back().Id, _links.back().InputId, _links.back().OutputId);
                }

                // You may choose to reject connection between these nodes
                // by calling ed::RejectNewItem(). This will allow editor to give
                // visual feedback by changing link thickness and color.
            }
        }
    }
    ed::EndCreate(); // Wraps up object creation action handling.


    // Handle deletion action
    if (ed::BeginDelete())
    {
        // There may be many links marked for deletion, let's loop over them.
        ed::LinkId deletedLinkId;
        while (ed::QueryDeletedLink(&deletedLinkId))
        {
            // If you agree that link can be deleted, accept deletion.
            if (ed::AcceptDeletedItem())
            {
                // Then remove link from your data.
                for (auto& link : _links)
                {
                    if (link.Id == deletedLinkId)
                    {
                        _links.erase(&link);
                        break;
                    }
                }
            }

            // You may reject link deletion by calling:
            // ed::RejectDeletedItem();
        }
    }
    ed::EndDelete(); // Wrap up deletion action

    ed::End();
    if (ImGui::BeginPopupContextItem("sdfxaas", ImGuiPopupFlags_MouseButtonMiddle)) {
        if (ImGui::Button("Sphere")) {
            _nodes.push_back(std::make_unique<Node_Sphere>());
            on_tree_change();
        }
        ImGui::EndPopup();
    }
    ImGui::End();
}
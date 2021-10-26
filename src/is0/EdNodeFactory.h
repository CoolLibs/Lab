#pragma once

#include <entt/entt.hpp>
#include "OldEdNode.h"

namespace EdNodeFactory {

inline int NextId()
{
    static int id = 1;
    return id++;
}

inline std::string fn_signature(const std::string& name)
{
    return "float " + name + "(vec3 pos)";
}

inline std::string fn_declaration(const std::string& signature)
{
    return signature + ";\n";
}

inline std::string fn_implementation(const std::string& signature, const std::string& body)
{
    return signature + " {\n" + body + "\n}\n";
}

inline entt::entity node(entt::registry& R, const std::string& name, CodeGenerator gen_source_code, UiWidgets show_widgets)
{
    entt::entity e       = R.create();
    std::string  fn_name = name + "__" + std::to_string(static_cast<std::uint32_t>(e));
    R.emplace<OldEdNode>(e,
                         /*name             */ name,
                         /*fn_name          */ fn_name,
                         /*node_id          */ static_cast<ed::NodeId>(NextId()),
                         /*fn_declaration   */ fn_declaration(fn_signature(fn_name)),
                         /*fn_implementation*/ "",
                         /*gen_source_code  */ gen_source_code,
                         /*show_widgets     */ show_widgets);
    R.emplace<IsTerminalNode>(e);
    return e;
}

inline entt::entity shape(entt::registry& R, const std::string& name, CodeGenerator fn_body_generator, UiWidgets show_widgets)
{
    entt::entity e = node(R, name, fn_body_generator, show_widgets);
    R.emplace<ShapeNode>(e, OutputPinSingle(NextId()));
    return e;
}

inline entt::entity modifier(entt::registry& R, const std::string& name, CodeGenerator fn_body_generator, UiWidgets show_widgets)
{
    entt::entity e = node(R, name, fn_body_generator, show_widgets);
    R.emplace<ModifierNode>(e,
                            ///*output_node*/ entt::null,
                            /*input_pin  */ InputPinSingle(NextId()),
                            /*output_pin */ OutputPinSingle(NextId()));
    return e;
}

struct Sphere {
    float radius;
};

inline entt::entity sphere(entt::registry& R)
{
    entt::entity e = shape(
        R,
        "Sphere",
        [&](entt::entity e) {
            const auto& sphere = R.get<Sphere>(e);
            return "return length(pos) - " + std::to_string(sphere.radius) + ";";
        },
        [&](entt::entity e) {
            auto& sphere = R.get<Sphere>(e);
            ImGui::PushID(static_cast<int>(e));
            ImGui::SetNextItemWidth(200.f);
            bool b = ImGui::SliderFloat("radius", &sphere.radius, 0.f, 10.f);
            ImGui::PopID();
            return b;
        });
    R.emplace<Sphere>(e, 1.f);
    return e;
}

struct Cube {
    glm::vec3 radii;
};

inline entt::entity cube(entt::registry& R)
{
    return shape(
        R,
        "Cube",
        [](entt::entity e) { return R"V0G0N( 
	vec3 q = abs(pos) - 1.; 
	return length(max(q, 0.)) + min(max(q.x, max(q.y, q.z)), 0.); 
	)V0G0N"; },
        [&](entt::entity e) {
            return false;
        });
}

struct Transform {
    glm::vec3 pos;
    glm::vec3 scale;
};

inline entt::entity transform(entt::registry& R, EdNodeEditor& node_enditor)
{
    entt::entity e = modifier(
        R,
        "Transform",
        [&](entt::entity e) {
            const auto&  transfo    = R.get<Transform>(e);
            entt::entity input_node = node_enditor.compute_node_connected_to_pin(R.get<ModifierNode>(e).input_pin.id);
            if (R.valid(input_node)) {
                std::string fn_name = R.get<OldEdNode>(input_node).fn_name;
                return "return " + fn_name + "(pos / vec3(" + std::to_string(transfo.scale.x) + ", " + std::to_string(transfo.scale.y) + ", " + std::to_string(transfo.scale.z) + ") - vec3(" + std::to_string(transfo.pos.x) + ", " + std::to_string(transfo.pos.y) + ", " + std::to_string(transfo.pos.z) + "));";
            }
            else {
                return std::string("return MAX_DIST;");
            }
        },
        [&](entt::entity e) {
            auto& transfo = R.get<Transform>(e);
            ImGui::PushID(static_cast<int>(e));
            ImGui::PushItemWidth(200.f);
            bool b = ImGui::SliderFloat3("position", glm::value_ptr(transfo.pos), -10.f, 10.f);
            b |= ImGui::SliderFloat3("scale", glm::value_ptr(transfo.scale), -10.f, 10.f);
            ImGui::PopItemWidth();
            ImGui::PopID();
            return b;
        });
    R.emplace<Transform>(e, glm::vec3(0.f), glm::vec3(1.f));
    return e;
}

struct RepeatInfinite {
    float extent;
};

inline entt::entity repeat_infinite(entt::registry& R, EdNodeEditor& node_enditor)
{
    entt::entity e = modifier(
        R,
        "Repeat",
        [&](entt::entity e) {
            const auto&  repeat     = R.get<RepeatInfinite>(e);
            entt::entity input_node = node_enditor.compute_node_connected_to_pin(R.get<ModifierNode>(e).input_pin.id);
            if (R.valid(input_node)) {
                std::string fn_name = R.get<OldEdNode>(input_node).fn_name;
                return "return " + fn_name + "((fract(pos / " + std::to_string(repeat.extent) + ")-0.5) * " + std::to_string(repeat.extent) + ");";
            }
            else {
                return std::string("return MAX_DIST;");
            }
        },
        [&](entt::entity e) {
            auto& repeat = R.get<RepeatInfinite>(e);
            ImGui::PushID(static_cast<int>(e));
            ImGui::SetNextItemWidth(200.f);
            bool b = ImGui::SliderFloat("extent", &repeat.extent, 0.f, 10.f);
            ImGui::PopID();
            return b;
        });
    R.emplace<RepeatInfinite>(e, 8.f);
    return e;
}
struct Twist {
    float k;
};

inline entt::entity twist(entt::registry& R, EdNodeEditor& node_enditor)
{
    entt::entity e = modifier(
        R,
        "Twist",
        [&](entt::entity e) {
            const auto&  twist_params = R.get<Twist>(e);
            entt::entity input_node   = node_enditor.compute_node_connected_to_pin(R.get<ModifierNode>(e).input_pin.id);
            if (R.valid(input_node)) {
                std::string fn_name = R.get<OldEdNode>(input_node).fn_name;
                return "const float k = " + std::to_string(twist_params.k) + ";" +
                       R"V0G0N( 
    float c = cos(k*pos.y);
    float s = sin(k*pos.y);
    mat2  m = mat2(c,-s,s,c);
    vec3  q = vec3(m*pos.xz,pos.y);
    return )V0G0N" + fn_name +
                       "(q);";
            }
            else {
                return std::string("return MAX_DIST;");
            }
        },
        [&](entt::entity e) {
            auto& twist_params = R.get<Twist>(e);
            ImGui::PushID(static_cast<int>(e));
            ImGui::SetNextItemWidth(200.f);
            bool b = ImGui::SliderFloat("twist", &twist_params.k, -0.02f, 0.02f);
            ImGui::PopID();
            return b;
        });
    R.emplace<Twist>(e, 0.f);
    return e;
}

} // namespace EdNodeFactory
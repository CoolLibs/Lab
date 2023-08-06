from dataclasses import dataclass
from typing import Optional, List, Any


@dataclass
class NodeKind:
    name_in_code: str
    user_facing_name: str
    color_srgb: str
    generate_compute_color_check: bool = True


def all_node_kinds():
    return [
        NodeKind(
            name_in_code="curve",
            user_facing_name="Curve",
            color_srgb="0.6666666865348816f, 0.14901961386203767f, 0.6784313917160034f",
        ),
        NodeKind(
            name_in_code="shape_2D",
            user_facing_name="Shape 2D",
            color_srgb="0.08235294371843338f, 0.5411764979362488f, 0.4588235318660736f",
        ),
        NodeKind(
            name_in_code="shape_3D",
            user_facing_name="Shape 3D",
            color_srgb="0.8313725590705872f, 0.5803921818733215f, 0.08627451211214066f",
        ),
        NodeKind(
            name_in_code="image",
            user_facing_name="Image",
            color_srgb="0.3019607961177826f, 0.4470588266849518f, 0.8901960849761963f",
        ),
        NodeKind(
            name_in_code="greyscale",
            user_facing_name="Greyscale Image",
            color_srgb="0.49803921580314639f, 0.49803921580314639f, 0.49803921580314639f",
        ),
        NodeKind(
            name_in_code="modifier_2D",
            user_facing_name="2D Modifier",
            color_srgb="1.f, 1.f, 1.f",
        ),
        NodeKind(
            name_in_code="miscellaneous",
            user_facing_name="Miscellaneous",
            color_srgb="0.8666666746139526f, 0.3176470696926117f, 0.19607843458652497f",
            generate_compute_color_check=False,
        ),
    ]


def node_colors_declaration():
    def code(kind: NodeKind):
        return f"auto {kind.name_in_code}() -> Cool::Color;"

    return "\n".join(map(code, all_node_kinds()))


def node_colors_definition():
    def code(kind: NodeKind):
        return f"""
            auto {kind.name_in_code}() -> Cool::Color
            {{
                return Cool::Color::from_srgb({{{kind.color_srgb}}});
            }}
        """

    return "\n".join(map(code, all_node_kinds()))


def compute_function_color():
    def code(kind: NodeKind):
        if not kind.generate_compute_color_check:
            return ""
        return f"""
            if (is_{kind.name_in_code}(signature))
                return NodeColor::{kind.name_in_code}();
        """

    return "\n".join(map(code, all_node_kinds()))


def snake_to_pascal(snake_case: str):
    words = snake_case.split("_")
    return "".join(f"{word[0].capitalize()}{word[1:]}" for word in words)


def node_kind_enum():
    def code(kind: NodeKind):
        return f"{snake_to_pascal(kind.name_in_code)},"

    return "\n".join(map(code, all_node_kinds()))


def imgui_node_kind_dropdown():
    def code(kind: NodeKind):
        return f"{kind.user_facing_name}"

    string = "\\0".join(map(code, all_node_kinds())) + "\\0\\0"

    return f"""
    int tmp = static_cast<int>(*node_kind);
    bool b = ImGui::Combo(label, &tmp, "{string}");
    if (b)
        *node_kind = static_cast<NodeKind>(tmp);
    return b;
    """


def node_kind_color():
    def code(kind: NodeKind):
        return f"case NodeKind::{snake_to_pascal(kind.name_in_code)}: return NodeColor::{kind.name_in_code}();"

    return "\n".join(map(code, all_node_kinds()))


if __name__ == "__main__":
    # HACK: Python doesn't allow us to import from a parent folder (e.g. tooling.generate_files)
    # So we need to add the path manually to sys.path
    import os
    import sys
    from pathlib import Path

    sys.path.append(
        os.path.join(Path(os.path.abspath(__file__)).parent.parent.parent, "tooling")
    )
    # End of HACK
    import generate_files

    generate_files.generate(
        folder="generated_node_kind",
        files=[
            node_colors_declaration,
            node_colors_definition,
            compute_function_color,
            node_kind_enum,
            imgui_node_kind_dropdown,
            node_kind_color,
        ],
    )

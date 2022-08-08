#include "TestVariables.h"
#include <Cool/Variables/Variables.h>
#include <imgui/imgui.h>
#include <vector>

namespace Lab {

void test_variables()
{
    static Cool::Variable<Cool::Angle> angle{
        "Angle",
        0.278_turn};
    static Cool::Variable<Cool::Hue> hue{
        "Hue",
        Cool::Hue{0.5f}};
    static Cool::Variable<Cool::RgbColor> color{
        "Color",
        Cool::RgbColor{glm::vec3{0.f, 1.f, 0.5f}}};
    static Cool::Variable<Cool::Direction2D> direction{
        "Direction",
        Cool::Direction2D{glm::vec2{0.f, 10.f}}};
    static Cool::Variable<glm::vec4> vector4{
        "Vector4",
        glm::vec4{0.f, 1.f, 0.5f, 8.f}};
    static Cool::Variable<glm::vec3> vector3{
        "Vector3",
        glm::vec3{1.f, 1.5f, 8.f}};
    // static Cool::Variable<Cool::ColorPalette> color_palette{
    //     "Color Palette",
    //     Cool::ColorPalette{
    //         std::vector{
    //             Cool::RgbColor{glm::vec3{0.f, 1.f, 0.75f}},
    //             Cool::RgbColor{glm::vec3{1.f, 1.f, 0.5f}},
    //             Cool::RgbColor{glm::vec3{1.f, 0.f, 0.5f}}}}};

    Cool::imgui(angle);
    Cool::imgui(color);
    Cool::imgui(hue);
    Cool::imgui(direction);
    Cool::imgui(vector3);
    Cool::imgui(vector4);
    // Cool::imgui(color_palette);
}

} // namespace Lab
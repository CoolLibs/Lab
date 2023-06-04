#include "NodeColor.h"

namespace Lab {

namespace NodeColor {
auto curve() -> Cool::Color
{
    return Cool::Color::from_srgb({0.6666666865348816f, 0.14901961386203767f, 0.6784313917160034f});
}
auto shape_2D() -> Cool::Color
{
    return Cool::Color::from_srgb({0.08235294371843338f, 0.5411764979362488f, 0.4588235318660736f});
}
auto shape_3D() -> Cool::Color
{
    return Cool::Color::from_srgb({0.8313725590705872f, 0.5803921818733215f, 0.08627451211214066f});
}
auto miscellaneous() -> Cool::Color
{
    return Cool::Color::from_srgb({0.8666666746139526f, 0.3176470696926117f, 0.19607843458652497f});
}
} // namespace NodeColor

auto compute_node_color(FunctionSignature signature) -> Cool::Color
{
    if (is_curve(signature))
        return NodeColor::curve();

    if (is_shape_2D(signature))
        return NodeColor::shape_2D();

    if (is_shape_3D(signature))
        return NodeColor::shape_3D();

    return NodeColor::miscellaneous();
}

} // namespace Lab
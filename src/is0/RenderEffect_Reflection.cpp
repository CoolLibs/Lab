#include "RenderEffect_Reflection.h"

std::string reflectionParameters(const RenderEffect_Reflection& reflection)
{
    return "const float IOR = " + std::to_string(*reflection.strenght) + ";\n\n";
};
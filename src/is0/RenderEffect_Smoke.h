#pragma once

#include <Cool/Parameter/Parameter.h>

struct RenderEffect_Smoke {
    Cool::Parameter::Float ABSORPTION_COEFFICIENT{{.name          = "Absorption Coefficient",
                                                   .default_value = 2.5f,
                                                   .min_value     = 0.01f,
                                                   .max_value     = 5.f}};

    // Reduce the following values to enhance performance

    Cool::Parameter::Float MARCH_MULTIPLIER{{.name          = "March Multiplier",
                                             .default_value = 0.5f,
                                             .min_value     = 0.01f,
                                             .max_value     = 3.f}};

    Cool::Parameter::Float MAX_VOLUME_MARCH_STEPS{{.name          = "Max Volume March Steps",
                                                   .default_value = 50.f,
                                                   .min_value     = 20.f,
                                                   .max_value     = 75.f}};

    bool is_active = true;
};

bool smoke_imgui_window(RenderEffect_Smoke& prop);

namespace CodeGen {

std::string code_gen_smoke_parameters(const RenderEffect_Smoke& parameters);
std::string IntersectVolumetricCodeGen();
std::string GetLightVisiblityCodeGen();
std::string Render();
std::string SmokeRenderer(const RenderEffect_Smoke& p);
std::string addSmoke(const RenderEffect_Smoke& p);

} // namespace CodeGen

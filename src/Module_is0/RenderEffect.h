#pragma once

#include <Cool/Parameter/ParameterList.h>
#include "BaseCode.h"
struct RenderEffect {
    BaseCode base;
    bool     is_active = true;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Effect", base),
                cereal::make_nvp("Enabled", is_active));
    }
};
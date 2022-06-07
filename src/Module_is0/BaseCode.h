#pragma once

#include <Cool/Parameter/ParameterList.h>

struct BaseCode {
    std::string         name;
    std::string         extra_code;
    std::string         parameters_declaration;
    std::string         code;
    Cool::ParameterList parameters;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Effect", name),
                cereal::make_nvp("Parameters", parameters));
    }
};

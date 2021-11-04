#pragma once

#include "Id.h"

struct Link {
    LinkId id{};
    PinId  from_pin_id;
    PinId  to_pin_id;

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Link id", id),
                cereal::make_nvp("From pin", from_pin_id),
                cereal::make_nvp("To pin", to_pin_id));
    }
};
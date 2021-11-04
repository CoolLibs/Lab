#pragma once

#include <Cool/Random/Random.h>

template<int PhantomIdentifier>
class Id {
public:
    Id()
        : _id{Cool::Random::integer()} {}
    explicit Id(int id)
        : _id{id} {}

    int operator*() const { return _id; }
        operator int() const { return _id; }

private:
    int _id;

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Id", _id));
    }
};

using NodeId = Id<0>;
using LinkId = Id<1>;
using PinId  = Id<2>;
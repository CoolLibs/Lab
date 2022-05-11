#pragma once
#include <reg/reg.hpp>

namespace Lab {

template<typename T>
class InputSlot {
public:
    reg::Id<T> id;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(id);
    }
};

} // namespace Lab
#pragma once
#include <reg/reg.hpp>

namespace Lab {

template<typename T>
class InputSlot {
public:
    explicit InputSlot(std::string_view name)
        : _name{name}
    {
    }

    std::string_view name() const { return _name; }

public: // This section is used by the InputProvider to do its job
    friend class InputProvider;
    friend class Ui;
    reg::Id<T> id;

private: // This section is really private
    std::string _name;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(id);
    }
};

using AnyInputSlotRef = std::variant<
    std::reference_wrapper<InputSlot<float>>,
    std::reference_wrapper<InputSlot<glm::vec3>>>;
using AnyInputSlotRefToConst = std::variant<
    std::reference_wrapper<const InputSlot<float>>,
    std::reference_wrapper<const InputSlot<glm::vec3>>>;

} // namespace Lab
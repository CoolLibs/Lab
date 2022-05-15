#pragma once
#include <reg/reg.hpp>

namespace Lab {

/*
 * The InputSlot is an opaque type that represent a request for a value. 
 * The actual value is provided by the InputProvider.
 */
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

} // namespace Lab
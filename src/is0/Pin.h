#pragma once

#include <Cool/Uuid/Uuid.h>
#include "Ed.h"

class Pin {
public:
    Pin()
        : _id{static_cast<ed::PinId>(Cool::Uuid{})}
    {
    }

    ed::PinId           id() const { return _id; }
    virtual ed::PinKind kind() const = 0;
    virtual void        show() const = 0;

private:
    ed::PinId _id;

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(_id);
    }
};

class PinSdfIn : public Pin {
public:
    ed::PinKind kind() const override
    {
        return ed::PinKind::Input;
    }

    void show() const override
    {
        ed::BeginPin(id(), kind());
        ImGui::Text("IN->");
        ed::EndPin();
    }

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::base_class<Pin>(this));
    }
};

class PinSdfOut : public Pin {
public:
    ed::PinKind kind() const override
    {
        return ed::PinKind::Output;
    }

    void show() const override
    {
        ed::BeginPin(id(), kind());
        ImGui::Text("OUT->");
        ed::EndPin();
    }

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::base_class<Pin>(this));
    }
};

namespace ax::NodeEditor {

template<class Archive>
void save(Archive& archive, const PinId& id)
{
    archive(cereal::make_nvp("id", id.Get()));
}
template<class Archive>
void load(Archive& archive, PinId& id)
{
    uintptr_t val;
    archive(val);
    id = ed::PinId{val};
}

} // namespace ax::NodeEditor
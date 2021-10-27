#pragma once

#include <Cool/Uuid/Uuid.h>
#include "Ed.h"
#include "EdSerialization.h"

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
        archive(cereal::make_nvp("id", _id));
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
        archive(cereal::make_nvp("Pin", cereal::base_class<Pin>(this)));
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
        archive(cereal::make_nvp("Pin", cereal::base_class<Pin>(this)));
    }
};
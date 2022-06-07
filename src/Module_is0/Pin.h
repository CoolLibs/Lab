#pragma once

#include <imnodes/imnodes.h>
#include "Id.h"

enum class PinKind {
    Input,
    Output
};

class Pin {
public:
    PinId id() const { return _id; }
    virtual ~Pin()               = default;
    virtual PinKind kind() const = 0;
    virtual void    show() const = 0;

private:
    PinId _id;

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
    PinKind kind() const override
    {
        return PinKind::Input;
    }

    void show() const override
    {
        ImNodes::BeginInputAttribute(id());
        ImGui::Dummy({0.f, 10.f}); // Makes sure that consecutive input pins are not too close to each other
        ImNodes::EndInputAttribute();
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
    PinKind kind() const override
    {
        return PinKind::Output;
    }

    void show() const override
    {
        ImNodes::BeginOutputAttribute(id());
        ImNodes::EndOutputAttribute();
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
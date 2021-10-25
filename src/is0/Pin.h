#pragma once

#include <Cool/Uuid/Uuid.h>
#include "Ed.h"

class Pin {
public:
    Pin()
        : _id{static_cast<ed::PinId>(Cool::Uuid{})}
    {
    }

    ed::PinId    id() const { return _id; }
    virtual void show() const = 0;

private:
    ed::PinId _id;
};

class PinSdfIn : public Pin {
public:
    void show() const override
    {
        ed::BeginPin(id(), ed::PinKind::Input);
        ImGui::Text("IN->");
        ed::EndPin();
    }
};

class PinSdfOut : public Pin {
public:
    void show() const override
    {
        ed::BeginPin(id(), ed::PinKind::Output);
        ImGui::Text("OUT->");
        ed::EndPin();
    }
};
#pragma once

#include "CommandCore/Command.h"
#include "CommandCore/ReversibleCommand.h"
#include "CommandCore/command_to_string.h"

namespace Lab {

class CommandLogger {
public:
    CommandLogger() = default;

    void push(const Command& command)
    {
        push(command_to_string(command));
    }

    void push(const ReversibleCommand& command)
    {
        push(command_to_string(command));
    }

    void imgui_show()
    {
        for (const auto& text : _texts) {
            ImGui::Text("%s", text.c_str());
        }
        if (_should_scroll_down) {
            ImGui::SetScrollHereY(1.f);
            _should_scroll_down = false;
        }
    }

private:
    void push(const std::string& text)
    {
        _texts.push_back(text);
        _should_scroll_down = true;
        if (_texts.size() > 100) {
            _texts.pop_front();
        }
    }

private:
    std::list<std::string> _texts{};
    bool                   _should_scroll_down = true;
};

} // namespace Lab
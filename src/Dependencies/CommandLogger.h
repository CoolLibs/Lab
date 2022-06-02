#pragma once

#include "Commands/Commands.h"
#include "Commands/command_to_string.h"

namespace Lab {

class CommandLogger {
public:
    CommandLogger() = default;

    void push(const Command& command)
    {
        _texts.push_back(command_to_string(command));
        _should_scroll_down = true;
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
    std::vector<std::string> _texts{};
    bool                     _should_scroll_down = true;
};

} // namespace Lab
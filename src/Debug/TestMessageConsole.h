#pragma once

#include <Cool/MessageConsole/MessageConsole.h>

namespace Lab {

class TestMessageConsole {
public:
    void imgui_window(Cool::MessageConsole& message_console)
    {
        ImGui::Begin("Test Message Console");
        ImGui::NewLine();
        imgui(_message1, message_console);
        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();
        imgui(_message2, message_console);
        ImGui::End();
    }

private:
    struct Message {
        std::string           category{"Test"};
        std::string           message{"Hello!"};
        Cool::MessageSeverity severity{Cool::MessageSeverity::Error};
        Cool::MessageId       id{};
    };

    void imgui(Message& message, Cool::MessageConsole& message_console)
    {
        ImGui::PushID(&message);
        if (ImGui::Button("Clear"))
        {
            message_console.clear(message.id);
        }
        if (message_console.should_highlight(message.id))
        {
            ImGui::SameLine();
            ImGui::Text("<-- CLICK HERE");
        }
        if (ImGui::Button("Send"))
        {
            message_console.send(message.id, {
                                                 .category         = message.category,
                                                 .detailed_message = message.message,
                                                 .severity         = message.severity,
                                             });
        }
        ImGui::InputText("Category", &message.category);
        ImGui::InputText("Message", &message.message);
        ImGui::Combo("Severity", (int*)&message.severity, "Info\0Warning\0Error\0\0");

        ImGui::PopID();
    }

private:
    Message _message1;
    Message _message2{"Test 2", "Hello! 2", Cool::MessageSeverity::Warning};
};

} // namespace Lab

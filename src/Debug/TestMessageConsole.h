#pragma once

#include <Cool/Log/ToUser.h>
#include <Cool/MessageConsole/MessageConsole.h>

namespace Lab {

class TestMessageConsole {
public:
    void imgui_window(Cool::MessageConsole& message_console)
    {
        if (should_bring_window_to_front)
        {
            should_bring_window_to_front = false;
            ImGui::SetNextWindowToFront();
        }
        ImGui::Begin("Test Message Console");
        ImGui::NewLine();
        for (auto& message : _messages)
        {
            imgui(message, message_console);
            ImGui::NewLine();
            ImGui::Separator();
            ImGui::NewLine();
        }
        if (ImGui::Button("Add a new message ID"))
        {
            _messages.push_back({.message = std::to_string(next_message_number++)});
            _messages.back().send_to(message_console);
        }
        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();
        if (ImGui::Button("Send to the legacy console"))
        {
            Cool::Log::ToUser::info("Test", "Hello World");
        }
        ImGui::End();
    }

private:
    struct Message {
        std::string           category{"Test"};
        std::string           message{"Hello!"};
        Cool::MessageSeverity severity{Cool::MessageSeverity::Error};
        Cool::MessageId       id{};

        void send_to(Cool::MessageConsole& message_console)
        {
            message_console.send(id, {
                                         .category         = category,
                                         .detailed_message = message,
                                         .severity         = severity,
                                     });
        }
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
            ImGui::SetScrollHereY(0.5f);
            should_bring_window_to_front = true;
        }
        if (ImGui::Button("Send"))
        {
            message.send_to(message_console);
        }
        ImGui::InputText("Category", &message.category);
        ImGui::InputText("Message", &message.message);
        ImGui::Combo("Severity", (int*)&message.severity, "Info\0Warning\0Error\0\0");

        ImGui::PopID();
    }

private:
    std::vector<Message> _messages{
        {},
        {"Test 2", "Hello! 2", Cool::MessageSeverity::Warning}};

    bool   should_bring_window_to_front{false};
    size_t next_message_number{0};
};

} // namespace Lab

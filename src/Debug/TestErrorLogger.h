#pragma once

#include <Cool/Errors/ErrorId.h>

namespace Lab {

class TestErrorLogger {
public:
    void imgui_window(Cool::ErrorLogger& error_logger)
    {
        ImGui::Begin("Test Error Logger");
        if (error_logger.should_highlight(_id))
        {
            ImGui::Text("!!!Click below!!!");
        }
        if (ImGui::Button("Clear Error"))
        {
            error_logger.clear(_id);
        }
        if (ImGui::Button("Send Error"))
        {
            error_logger.refresh(_id, {_error_message});
        }
        ImGui::SameLine();
        ImGui::InputText("##1", &_error_message);
        ImGui::End();
    }

private:
    Cool::ErrorId _id{};
    std::string   _error_message{"Problem!"};
};

} // namespace Lab

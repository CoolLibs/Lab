#include "didYouKnowModal.hpp"
#include <imgui.h>

void DidYouKnowModal::open()
{
    if (!_has_been_opened)
    {
        _has_been_opened = true;
        ImGui::OpenPopup(_id.c_str());
        return;
    }
}

void DidYouKnowModal::prepare_next_tip()
{
    _current_tip_index++;
    if (_current_tip_index >= _all_tips.size())
        _current_tip_index = 0;
    _text = _all_tips[_current_tip_index];
}

bool DidYouKnowModal::is_open() const
{
    return ImGui::BeginPopupModal(_id.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize);
}

void DidYouKnowModal::display_modal_content()
{
    ImGui::Text(_text.c_str());
    ImGui::Separator();

    if (ImGui::Button("OK", ImVec2(120, 0)))
    {
        ImGui::CloseCurrentPopup();
        prepare_next_tip();
    }

    ImGui::SameLine();
    ImGui::SetItemDefaultFocus();

    if (ImGui::Button("Show all tips", ImVec2(120, 0)))
    {
        ImGui::OpenPopup("All tips");
    }

    all_tips();
    ImGui::EndPopup();
}

void DidYouKnowModal::all_tips()
{
    if (ImGui::BeginPopupModal("All tips", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        for (const auto& tip : _all_tips)
        {
            ImGui::Text(tip.c_str());
            ImGui::Separator();
        }
        if (ImGui::Button("Got it !"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

void test_did_you_know(DidYouKnowModal& _did_you_know_)
{
    _did_you_know_.open();

    if (_did_you_know_.is_open())
    {
        _did_you_know_.display_modal_content();
    }
}

void debug_did_you_know(DidYouKnowModal& _did_you_know_)
{
    ImGui::Begin("Debug did you know");
    if (ImGui::Button("Test DidYouKnow"))
        _did_you_know_._has_been_opened = false;
    ImGui::End();
}

#include "NodeEditorSerialization.h"
#include <Cool/ImGui/ImGuiExtras.h>

void pick_file_path_to_save(std::string& _folder_path_for_save, std::string& _file_name)
{
    ImGui::InputText("Save Name", &_file_name);
    ImGui::InputText("Save Folder", &_folder_path_for_save);
    ImGui::SameLine();
    Cool::ImGuiExtras::open_folder_dialog(&_folder_path_for_save, _folder_path_for_save);
}
#include "NodeEditor.h"

NodeEditor::NodeEditor()
	: _context(ed::CreateEditor())
{
}

NodeEditor::~NodeEditor() {
	ed::DestroyEditor(_context);
}

void NodeEditor::ImGui_window()
{
    ImGui::Begin("Nodes");
    ed::SetCurrentEditor(_context);
    ed::Begin("My Editor");
    int uniqueId = 1;

    // Start drawing nodes.
    ed::BeginNode(uniqueId++);
        ImGui::Text("Node A");
        ed::BeginPin(uniqueId++, ed::PinKind::Input);
            ImGui::Text("-> In");
        ed::EndPin();
        ImGui::SameLine();
        ed::BeginPin(uniqueId++, ed::PinKind::Output);
            ImGui::Text("Out ->");
        ed::EndPin();
    ed::EndNode();

    ed::End();
    ImGui::End();
}
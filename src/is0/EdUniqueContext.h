#pragma once

#include <imnodes/imnodes.h>

class EdUniqueContext {
public:
    EdUniqueContext()
    {
        ImNodes::CreateContext();
        ImNodes::LoadCurrentEditorStateFromIniFile("is0_node_editor.ini");
    }
    ~EdUniqueContext()
    {
        if (!_was_moved) {
            ImNodes::SaveCurrentEditorStateToIniFile("is0_node_editor.ini");
            ImNodes::DestroyContext();
        }
    }
    EdUniqueContext(const EdUniqueContext&) = delete;
    EdUniqueContext& operator=(const EdUniqueContext&) = delete;
    EdUniqueContext(EdUniqueContext&& rhs) noexcept
    {
        rhs._was_moved = true;
    };
    EdUniqueContext& operator=(EdUniqueContext&& rhs) noexcept
    {
        rhs._was_moved = true;
        return *this;
    }

private:
    bool _was_moved = false;
};
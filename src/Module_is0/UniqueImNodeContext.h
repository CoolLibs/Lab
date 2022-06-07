#pragma once

#include <imnodes/imnodes.h>

class UniqueImNodeContext {
public:
    UniqueImNodeContext()
        : _context{ImNodes::CreateContext()}
    {
        static const bool b                                   = true;
        ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &b;
        ImNodes::GetIO().AltMouseButton                       = ImGuiMouseButton_Right;
        ImNodes::GetIO().EmulateThreeButtonMouse.Modifier     = &ImGui::GetIO().KeyShift;
    }
    ~UniqueImNodeContext()
    {
        if (_context) {
            ImNodes::DestroyContext(_context);
        }
    }
    UniqueImNodeContext(const UniqueImNodeContext&) = delete;
    UniqueImNodeContext& operator=(const UniqueImNodeContext&) = delete;
    UniqueImNodeContext(UniqueImNodeContext&& rhs) noexcept
        : _context{rhs._context}
    {
        rhs._context = nullptr;
    };
    UniqueImNodeContext& operator=(UniqueImNodeContext&& rhs) noexcept
    {
        if (this != &rhs) {
            ImNodes::DestroyContext(_context);
            _context     = rhs._context;
            rhs._context = nullptr;
        }
        return *this;
    }

    ImNodesContext&       operator*() { return *_context; }
    ImNodesContext*       operator->() { return _context; }
    const ImNodesContext& operator*() const { return *_context; }
    const ImNodesContext* operator->() const { return _context; }

private:
    ImNodesContext* _context;
};
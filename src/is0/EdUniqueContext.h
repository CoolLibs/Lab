#pragma once

#include "Ed.h"

class EdUniqueContext {
public:
    EdUniqueContext()
        : _context{ed::CreateEditor()}
    {
    }
    ~EdUniqueContext()
    {
        if (_context) {
            ed::DestroyEditor(_context);
        }
    }
    EdUniqueContext(const EdUniqueContext&) = delete;
    EdUniqueContext& operator=(const EdUniqueContext&) = delete;
    EdUniqueContext(EdUniqueContext&& rhs)
        : _context{rhs._context}
    {
        rhs._context = nullptr;
    };
    EdUniqueContext& operator=(EdUniqueContext&& rhs)
    {
        if (this != &rhs) {
            _context     = rhs._context;
            rhs._context = nullptr;
        }
        return *this;
    }

    ed::EditorContext&       operator*() { return *_context; }
    ed::EditorContext*       operator->() { return _context; }
    const ed::EditorContext& operator*() const { return *_context; }
    const ed::EditorContext* operator->() const { return _context; }

private:
    ed::EditorContext* _context;
};
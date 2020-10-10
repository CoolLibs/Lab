#pragma once

#include <assert.h>
#include <debug_break/debug_break.h>

/**
 * @brief Assertion and logger handling for opengl functions
 */
#ifndef NDEBUG
    #define BreakAssert(x) if (!x) { debug_break(); assert(false); }
    #define GLCall(x) glexp::clear(); x; BreakAssert(glexp::doesFunctionWork(#x, __FILE__, __LINE__))
#else
    #define GLCall(x) x
#endif

namespace glexp {
    /**
     * @brief Clear the OpenGl error buffer
     */
    void clear();

    /**
     * @brief Print OpenGl errors to the console if any
     */
    bool doesFunctionWork(const char* functionName, const char* filename, int line);

    /**
     * @brief Transform error enum to text
     * @return char const* - Text corresponding to OpenGl error
     */
    char const* glErrorString(GLenum const err);
}
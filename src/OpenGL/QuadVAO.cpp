#include "QuadVAO.h"

QuadVAO::QuadVAO(bool bWithUVs) {
    // Vertex array
    GLCall(glGenVertexArrays(1, &m_vaoID));
    GLCall(glBindVertexArray(m_vaoID));
    // Vertex buffer
    GLCall(glGenBuffers(1, &m_vboID));    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
    static float vertices[] = {
        -1.0f, -1.0f,
         3.0f, -1.0f,
        -1.0f,  3.0f
    };    
    static float verticesWithUVs[] = {
        -1.0f, -1.0f,  0.0f, 0.0f,
         3.0f, -1.0f,  2.0f, 0.0f,
        -1.0f,  3.0f,  0.0f, 2.0f
    };
    if (!bWithUVs) {
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    }
    else {
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(verticesWithUVs), verticesWithUVs, GL_STATIC_DRAW));
    }
    // Layout
    constexpr int VERTEX_ATTRIB_POS = 0;
    if (!bWithUVs) {
        GLCall(glEnableVertexAttribArray(VERTEX_ATTRIB_POS));
        GLCall(glVertexAttribPointer(VERTEX_ATTRIB_POS, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
    }
    else {
        constexpr int VERTEX_ATTRIB_UV = 1;
        GLCall(glEnableVertexAttribArray(VERTEX_ATTRIB_POS));
        GLCall(glVertexAttribPointer(VERTEX_ATTRIB_POS, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
        GLCall(glEnableVertexAttribArray(VERTEX_ATTRIB_UV));
        GLCall(glVertexAttribPointer(VERTEX_ATTRIB_UV, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
    }
}

QuadVAO::~QuadVAO() {
    GLCall(glDeleteBuffers(1, &m_vboID));
    GLCall(glDeleteVertexArrays(1, &m_vaoID));
}

void QuadVAO::bind() {
    GLCall(glBindVertexArray(m_vaoID));
}
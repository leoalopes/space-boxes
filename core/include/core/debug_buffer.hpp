#pragma once
#include <glad/glad.h>

#include "core/object_buffer.hpp"
#include "glm/fwd.hpp"

class DebugBuffer : public ObjectBuffer {
  public:
    DebugBuffer(const float *vertices, long size, glm::vec3 origin)
        : ObjectBuffer(vertices, size, GL_STATIC_DRAW, origin) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);
    }

    void draw() override {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(this->objectId);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
};

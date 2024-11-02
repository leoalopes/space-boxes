#pragma once
#include <glad/glad.h>

#include "core/object_buffer.hpp"
#include <array>
#include <glm/glm.hpp>

class CubeBuffer : public ObjectBuffer<const float> {
    static inline constexpr std::array<float, 180> vertices = {
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // Bottom-right
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // Top-right
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // Top-right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // Top-left
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left

        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // Bottom-left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // Bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // Top-right
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // Top-right
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // Top-left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // Bottom-left

        // Left face
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // Top-right
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // Top-left
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // Bottom-left
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // Bottom-left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // Bottom-right
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // Top-right

        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // Top-left
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // Top-right
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // Bottom-right
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // Bottom-right
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // Bottom-left
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // Top-left

        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // Top-left
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // Top-right
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // Bottom-right
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // Bottom-right
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // Bottom-left
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // Top-left

        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // Top-left
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // Top-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // Bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // Bottom-right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,  // Bottom-left
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f  // Top-left
    };

  public:
    CubeBuffer()
        : ObjectBuffer<const float>(
              this->vertices.data(), this->vertices.size() * sizeof(float),
              GL_STATIC_DRAW, glm::vec3(-0.5f, -0.5f, -0.5f)) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void draw() override {
        glBindVertexArray(this->objectId);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
};

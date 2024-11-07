#pragma once
#include <glad/glad.h>

#include "core/debug_buffer.hpp"
#include "glm/fwd.hpp"
#include <array>
#include <limits>
#include <memory>

class ColliderBuffer {
    std::array<float, 108> original;
    std::array<float, 108> vertices;
    std::unique_ptr<DebugBuffer> buffer;

    void setupBufferData(glm::vec3 origin, glm::vec3 dimensions) {
        // Back face
        // Bottom-left
        vertices.at(0) = origin.x;
        vertices.at(1) = origin.y;
        vertices.at(2) = origin.z;
        // Bottom-right
        vertices.at(3) = origin.x + dimensions.x;
        vertices.at(4) = origin.y;
        vertices.at(5) = origin.z;
        // Top-right
        vertices.at(6) = origin.x + dimensions.x;
        vertices.at(7) = origin.y + dimensions.y;
        vertices.at(8) = origin.z;
        // Top-right
        vertices.at(9) = origin.x + dimensions.x;
        vertices.at(10) = origin.y + dimensions.y;
        vertices.at(11) = origin.z;
        // Top-left
        vertices.at(12) = origin.x;
        vertices.at(13) = origin.y + dimensions.y;
        vertices.at(14) = origin.z;
        // Bottom-left
        vertices.at(15) = origin.x;
        vertices.at(16) = origin.y;
        vertices.at(17) = origin.z;

        // Front face
        // Bottom-left
        vertices.at(18) = origin.x;
        vertices.at(19) = origin.y;
        vertices.at(20) = origin.z + dimensions.z;
        // Bottom-right
        vertices.at(21) = origin.x + dimensions.x;
        vertices.at(22) = origin.y;
        vertices.at(23) = origin.z + dimensions.z;
        // Top-right
        vertices.at(24) = origin.x + dimensions.x;
        vertices.at(25) = origin.y + dimensions.y;
        vertices.at(26) = origin.z + dimensions.z;
        // Top-right
        vertices.at(27) = origin.x + dimensions.x;
        vertices.at(28) = origin.y + dimensions.y;
        vertices.at(29) = origin.z + dimensions.z;
        // Top-left
        vertices.at(30) = origin.x;
        vertices.at(31) = origin.y + dimensions.y;
        vertices.at(32) = origin.z + dimensions.z;
        // Bottom-left
        vertices.at(33) = origin.x;
        vertices.at(34) = origin.y;
        vertices.at(35) = origin.z + dimensions.z;

        // Left face
        // Top-right
        vertices.at(36) = origin.x;
        vertices.at(37) = origin.y + dimensions.y;
        vertices.at(38) = origin.z + dimensions.z;
        // Top-left
        vertices.at(39) = origin.x;
        vertices.at(40) = origin.y + dimensions.y;
        vertices.at(41) = origin.z;
        // Bottom-left
        vertices.at(42) = origin.x;
        vertices.at(43) = origin.y;
        vertices.at(44) = origin.z;
        // Bottom-left
        vertices.at(45) = origin.x;
        vertices.at(46) = origin.y;
        vertices.at(47) = origin.z;
        // Bottom-right
        vertices.at(48) = origin.x;
        vertices.at(49) = origin.y;
        vertices.at(50) = origin.z + dimensions.z;
        // Top-right
        vertices.at(51) = origin.x;
        vertices.at(52) = origin.y + dimensions.y;
        vertices.at(53) = origin.z + dimensions.z;

        // Right face
        // Top-left
        vertices.at(54) = origin.x + dimensions.x;
        vertices.at(55) = origin.y + dimensions.y;
        vertices.at(56) = origin.z + dimensions.z;
        // Top-right
        vertices.at(57) = origin.x + dimensions.x;
        vertices.at(58) = origin.y + dimensions.y;
        vertices.at(59) = origin.z;
        // Bottom-right
        vertices.at(60) = origin.x + dimensions.x;
        vertices.at(61) = origin.y;
        vertices.at(62) = origin.z;
        // Bottom-right
        vertices.at(63) = origin.x + dimensions.x;
        vertices.at(64) = origin.y;
        vertices.at(65) = origin.z;
        // Bottom-left
        vertices.at(66) = origin.x + dimensions.x;
        vertices.at(67) = origin.y;
        vertices.at(68) = origin.z + dimensions.z;
        // Top-left
        vertices.at(69) = origin.x + dimensions.x;
        vertices.at(70) = origin.y + dimensions.y;
        vertices.at(71) = origin.z + dimensions.z;

        // Bottom face
        // Top-left
        vertices.at(72) = origin.x;
        vertices.at(73) = origin.y;
        vertices.at(74) = origin.z;
        // Top-right
        vertices.at(75) = origin.x + dimensions.x;
        vertices.at(76) = origin.y;
        vertices.at(77) = origin.z;
        // Bottom-right
        vertices.at(78) = origin.x + dimensions.x;
        vertices.at(79) = origin.y;
        vertices.at(80) = origin.z + dimensions.z;
        // Bottom-right
        vertices.at(81) = origin.x + dimensions.x;
        vertices.at(82) = origin.y;
        vertices.at(83) = origin.z + dimensions.z;
        // Bottom-left
        vertices.at(84) = origin.x;
        vertices.at(85) = origin.y;
        vertices.at(86) = origin.z + dimensions.z;
        // Top-left
        vertices.at(87) = origin.x;
        vertices.at(88) = origin.y;
        vertices.at(89) = origin.z;

        // Top face
        // Top-left
        vertices.at(90) = origin.x;
        vertices.at(91) = origin.y + dimensions.y;
        vertices.at(92) = origin.z;
        // Top-right
        vertices.at(93) = origin.x + dimensions.x;
        vertices.at(94) = origin.y + dimensions.y;
        vertices.at(95) = origin.z;
        // Bottom-right
        vertices.at(96) = origin.x + dimensions.x;
        vertices.at(97) = origin.y + dimensions.y;
        vertices.at(98) = origin.z + dimensions.z;
        // Bottom-right
        vertices.at(99) = origin.x + dimensions.x;
        vertices.at(100) = origin.y + dimensions.y;
        vertices.at(101) = origin.z + dimensions.z;
        // Bottom-left
        vertices.at(102) = origin.x;
        vertices.at(103) = origin.y + dimensions.y;
        vertices.at(104) = origin.z + dimensions.z;
        // Top-left
        vertices.at(105) = origin.x;
        vertices.at(106) = origin.y + dimensions.y;
        vertices.at(107) = origin.z;

        this->original = vertices;
    }

  public:
    ColliderBuffer(glm::vec3 origin, glm::vec3 dimensions) {
        this->setupBufferData(origin, dimensions);
        this->buffer = std::make_unique<DebugBuffer>(
            this->vertices.data(), this->vertices.size() * sizeof(float),
            origin);
    }

    void update(glm::mat4 transform) {
        this->vertices = original;
        for (int i = 0; i < 108; i += 3) {
            glm::vec3 position{
                vertices.at(i),
                vertices.at(i + 1),
                vertices.at(i + 2),
            };
            position = transform * glm::vec4(position, 1.0f);

            vertices.at(i) = position.x;
            vertices.at(i + 1) = position.y;
            vertices.at(i + 2) = position.z;
        }

        this->buffer->update(vertices.data(),
                             this->vertices.size() * sizeof(float));
    }

    std::array<glm::vec3, 2> getBoundingBox() {
        glm::vec3 beginning{std::numeric_limits<float>::max()};
        glm::vec3 end{std::numeric_limits<float>::lowest()};

        for (int i = 0; i < 108; i += 3) {
            glm::vec3 point{vertices[i], vertices[i + 1], vertices[i + 2]};

            beginning = glm::min(beginning, point);
            end = glm::max(end, point);
        }

        return {beginning, end};
    }

    void draw() {
        if (!this->buffer) {
            return;
        }

        this->buffer->draw();
    }
};

#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>

const glm::vec3 DEFAULT_ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f);

template <typename Vertex> class ObjectBuffer {
  protected:
    unsigned int objectId;
    unsigned int bufferId;
    glm::vec3 modelOrigin;

  public:
    ObjectBuffer(Vertex *vertices, long size, GLenum usage = GL_STATIC_DRAW,
                 glm::vec3 modelOrigin = DEFAULT_ORIGIN)
        : modelOrigin(modelOrigin) {
        glGenVertexArrays(1, &this->objectId);
        glGenBuffers(1, &this->bufferId);

        glBindVertexArray(this->objectId);
        glBindBuffer(GL_ARRAY_BUFFER, this->bufferId);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
    }

    glm::vec3 getModelOrigin() const { return this->modelOrigin; }

    virtual void draw() = 0;
};

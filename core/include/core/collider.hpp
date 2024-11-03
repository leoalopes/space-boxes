#pragma once
#include "core/collider_buffer.hpp"
#include "core/object.hpp"
#include "core/shader.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <memory>

class Collider {
    Object *parent;
    Shader debugShader;
    std::unique_ptr<ColliderBuffer> buffer;

  public:
    Collider(Object *parent, glm::vec3 origin, glm::vec3 dimensions)
        : debugShader("assets/shaders/debug.vert",
                      "assets/shaders/debug.frag") {
        this->parent = parent;
        this->buffer = std::make_unique<ColliderBuffer>(origin, dimensions);
    }

    void debug(glm::mat4 projection, glm::mat4 view, glm::mat4 model) {
        this->debugShader.use();
        this->debugShader.setMatrix("projection", glm::value_ptr(projection));
        this->debugShader.setMatrix("view", glm::value_ptr(view));
        this->debugShader.setMatrix("model", glm::value_ptr(model));

        this->buffer->draw();
    }
};

#pragma once
#include "core/scene.hpp"
#include "core/transform.hpp"

class Scene;

class Object {
  protected:
    unsigned int objectId;
    Transform transform;
    Scene *scene;
    glm::mat4 projection;
    glm::mat4 view;

  public:
    virtual void update() {}
    virtual void draw() = 0;

    Transform *getTransform() { return &this->transform; }

    virtual void setViewMatrix(glm::mat4 matrix) { this->view = matrix; }
    virtual void setProjectionMatrix(glm::mat4 matrix) {
        this->projection = matrix;
    }

    Scene *getScene() const { return this->scene; }
    void setScene(Scene *scene) { this->scene = scene; }
};

#pragma once
#include "core/object_buffer.hpp"
#include "core/scene.hpp"
#include "core/transform.hpp"
#include "core/uuid.hpp"

class Collision;
class Scene;

class Object {
  protected:
    std::string uuid;
    std::string name;
    std::string type;
    Transform transform;
    Scene *scene;
    glm::mat4 projection;
    glm::mat4 view;

  public:
    virtual void update(std::vector<Collision> collisions) {}
    virtual void draw(bool isColliding) = 0;

    Object(std::string name, std::string type)
        : name(name), type(type), uuid(UUID::v4()) {}

    std::string getUUID() const { return this->uuid; }
    std::string getName() const { return this->name; }
    std::string getType() const { return this->type; }

    Transform *getTransform() { return &this->transform; }

    virtual void setViewMatrix(glm::mat4 matrix) { this->view = matrix; }
    virtual void setProjectionMatrix(glm::mat4 matrix) {
        this->projection = matrix;
    }

    Scene *getScene() const { return this->scene; }
    void setScene(Scene *scene) { this->scene = scene; }

    virtual ObjectBuffer *getBuffer() = 0;

    virtual bool isCollisionAware() { return false; }
};

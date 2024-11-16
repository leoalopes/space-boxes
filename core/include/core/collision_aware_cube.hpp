#pragma once
#include "core/collision_aware.hpp"
#include "core/cube.hpp"

class CollisionAwareCube : public Cube, public CollisionAware {
  public:
    CollisionAwareCube(std::string name, std::string type, CubeBuffer buffer,
                       Shader shader, std::vector<Texture> textures)
        : Cube(name, type, buffer, shader, textures), CollisionAware() {
        this->initializeCollider(this, glm::vec3(-0.5f, -0.5f, -0.5f),
                                 glm::vec3(1.0f, 1.0f, 1.0f),
                                 {"player", "bullet"});
    }

    void draw(bool isColliding) override {
        if (this->isDestroyed()) {
            return;
        }

        Cube::draw(isColliding);
        this->collider->debug(this->projection, this->view,
                              isColliding ? 1 : 0);
    }

    bool isCollisionAware() override { return true; }
};

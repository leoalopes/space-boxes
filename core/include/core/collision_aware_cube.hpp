#pragma once
#include "core/collision_aware.hpp"
#include "core/cube.hpp"

class CollisionAwareCube : public Cube, public CollisionAware {
  public:
    CollisionAwareCube(CubeBuffer buffer, Shader shader,
                       std::vector<Texture> textures)
        : Cube(buffer, shader, textures),
          CollisionAware(this, glm::vec3(-0.5f, -0.5f, -0.5f),
                         glm::vec3(1.0f, 1.0f, 1.0f)) {}

    void draw() override {
        Cube::draw();

        this->collider->debug(this->projection, this->view,
                              this->transform.getMatrix());
    }
};

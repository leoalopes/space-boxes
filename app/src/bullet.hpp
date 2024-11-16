#pragma once

#include "core/collision_aware.hpp"
#include "core/cube.hpp"

class Bullet : public Cube, public CollisionAware {
  private:
    const float speed = 50.0f;

  public:
    Bullet(CubeBuffer buffer, Shader shader, std::vector<Texture> textures)
        : Cube("bullet", "bullet", buffer, shader, textures), CollisionAware() {
        this->initializeCollider(this, glm::vec3(-0.5f, -0.5f, -0.5f),
                                 glm::vec3(1.0f, 1.0f, 1.0f),
                                 {"wall", "enemy"});
    }

    void update(std::vector<Collision> collisions) override {
        this->collider->updateBoundingBox();

        for (Collision collision : collisions) {
            if (collision.target->getType() == "enemy") {
                collision.target->destroy();
            }
        }

        if (!collisions.empty()) {
            this->destroy();
        }

        const glm::vec3 forwardVector = this->transform.getForwardVector();
        glm::vec3 location = this->transform.getLocation();
        location += forwardVector * speed * this->scene->getDeltaTime();
        this->transform.setLocation(location);
    }

    void draw(bool isColliding) override {
        Cube::draw(isColliding);

        this->collider->debug(this->projection, this->view,
                              isColliding ? 1 : 0);
    }

    bool isCollisionAware() override { return true; }
};

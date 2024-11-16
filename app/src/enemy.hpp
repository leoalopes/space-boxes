#pragma once

#include "core/collision_aware.hpp"
#include "core/cube.hpp"
#include "glm/geometric.hpp"
#include "player.hpp"
#include "randomizer.hpp"

const int directionChangeFrames = 30;

class Enemy : public Cube, public CollisionAware {
    std::array<glm::vec3, 2> bounds;
    Randomizer randomizer;
    glm::vec3 direction;
    double directionChangeDelay;
    Player *player;

    void setSpawn() {
        glm::vec3 playerLocation = this->player->getTransform()->getLocation();
        glm::vec3 location{0.0f};
        do {
            location = {this->randomizer.getRandomBetween(this->bounds[0].x,
                                                          this->bounds[1].x),
                        0.0f,
                        this->randomizer.getRandomBetween(this->bounds[0].z,
                                                          this->bounds[1].z)};
            /*std::cout << "Spawn: " << location.x << " " << location.z <<
             * '\n';*/
        } while (glm::length(location - playerLocation) < 15.0f);
        this->transform.setLocation(location);
        this->collider->updateBoundingBox();
    }

  public:
    Enemy(std::array<glm::vec3, 2> bounds, Player *player, CubeBuffer buffer,
          Shader shader, std::vector<Texture> textures)
        : Cube("enemy", "enemy", buffer, shader, textures), CollisionAware(),
          bounds(bounds), direction(0.0f), directionChangeDelay(0),
          player(player) {
        this->initializeCollider(this, glm::vec3(-0.5f, -0.5f, -0.5f),
                                 glm::vec3(1.0f, 1.0f, 1.0f),
                                 {"bullet", "player"});
        this->setSpawn();
    }

    void update(std::vector<Collision> collisions) override {
        if (this->directionChangeDelay > 0) {
            this->directionChangeDelay -= this->scene->getDeltaTime();
        }

        if (this->directionChangeDelay <= 0) {
            bool shouldRandomizeDirection = this->randomizer.getRandomBool(5);
            if (shouldRandomizeDirection) {
                this->direction = this->randomizer.getRandomDirection();
            } else {
                glm::vec3 playerLocation =
                    this->player->getTransform()->getLocation();
                this->direction = glm::normalize(playerLocation -
                                                 this->transform.getLocation());
            }
            this->directionChangeDelay =
                directionChangeFrames * this->scene->getDeltaTime();
        }

        glm::vec3 location = this->transform.getLocation();
        location += this->direction * 4.75f * this->scene->getDeltaTime();

        if (location.x <= this->bounds[0].x ||
            location.x >= this->bounds[1].x) {
            location.x = this->transform.getLocation().x;
        }

        if (location.z <= this->bounds[0].z ||
            location.z >= this->bounds[1].z) {
            location.z = this->transform.getLocation().z;
        }

        this->transform.setLocation(location);
        this->collider->updateBoundingBox();
    }

    bool isCollisionAware() override { return true; }
};

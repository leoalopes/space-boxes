#pragma once

#include "core/collision_aware_cube.hpp"
#include "glm/geometric.hpp"
#include <array>

const float visibilityThreshold = 2.0f;

class Wall : public CollisionAwareCube {
  public:
    using CollisionAwareCube::CollisionAwareCube;

    void draw(bool isColliding) override {
        Scene *scene = this->getScene();
        if (scene == nullptr) {
            return;
        }

        Camera *camera = scene->getCamera();
        if (camera == nullptr) {
            return;
        }

        Object *player = nullptr;
        std::vector<Object *> objects = scene->getObjects();
        for (int i = 0; i < objects.size(); i++) {
            if (objects.at(i)->getType() == "player") {
                player = objects.at(i);
                break;
            }
        }

        if (player == nullptr) {
            return;
        }

        glm::vec3 playerDiff =
            camera->getLocation() - player->getTransform()->getLocation();

        std::array<glm::vec3, 2> boundingBox =
            this->getCollider()->getBoundingBox();
        glm::vec3 wallPosition{(boundingBox.at(1).x + boundingBox.at(0).x) / 2,
                               (boundingBox.at(1).y + boundingBox.at(0).y) / 2,
                               (boundingBox.at(1).z + boundingBox.at(0).z) / 2};
        glm::vec3 wallHalfSize{(boundingBox.at(1).x - boundingBox.at(0).x) / 2,
                               (boundingBox.at(1).y - boundingBox.at(0).y) / 2,
                               (boundingBox.at(1).z - boundingBox.at(0).z) / 2};

        glm::vec3 wallToCamera = camera->getLocation() - wallPosition;
        glm::vec3 wallClosest =
            wallPosition +
            glm::clamp(wallToCamera, -wallHalfSize, wallHalfSize);

        glm::vec3 wallDiff = camera->getLocation() - wallClosest;

        if (glm::length(playerDiff) <=
            glm::length(wallDiff) + visibilityThreshold) {
            CollisionAwareCube::draw(isColliding);
        }
    }
};

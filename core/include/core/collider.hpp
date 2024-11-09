#pragma once
#include "core/collider_buffer.hpp"
#include "core/object.hpp"
#include "core/shader.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <memory>

class Collider {
    Object *parent;
    Shader debugShader;
    std::vector<std::string> collideWith;
    std::unique_ptr<ColliderBuffer> buffer;
    glm::vec3 origin;
    glm::vec3 dimensions;
    std::array<glm::vec3, 2> boundingBox;

  public:
    Collider(Object *parent, glm::vec3 origin, glm::vec3 dimensions,
             std::vector<std::string> collideWith)
        : origin(origin), dimensions(dimensions),
          debugShader("assets/shaders/debug.vert", "assets/shaders/debug.frag"),
          boundingBox({glm::vec3{0.0f}, glm::vec3{0.0f}}),
          collideWith(collideWith) {
        this->parent = parent;
        this->buffer = std::make_unique<ColliderBuffer>(origin, dimensions);
        this->updateBoundingBox();
    }

    void updateBoundingBox() {
        if (this->parent == nullptr) {
            std::cout << "Not updating bounding box, because parent is null"
                      << '\n';
            return;
        }

        glm::mat4 transform = this->parent->getTransform()->getMatrix();
        this->buffer->update(transform);
        this->boundingBox = this->buffer->getBoundingBox();
    }

    void debug(glm::mat4 projection, glm::mat4 view, int isColliding) {
        this->debugShader.use();
        this->debugShader.setMatrix("projection", glm::value_ptr(projection));
        this->debugShader.setMatrix("view", glm::value_ptr(view));
        this->debugShader.setInt("isColliding", isColliding);

        this->buffer->draw();
    }

    Object *getParent() { return this->parent; }

    glm::vec3 getOrigin() const { return this->origin; }
    glm::vec3 getDimensions() const { return this->dimensions; }

    bool checkCollision(Collider *targetCollider, glm::vec3 *direction) {
        if (this->getParent() == nullptr ||
            targetCollider->getParent() == nullptr) {
            return false;
        }

        if (!this->collideWith.empty() &&
            std::count(this->collideWith.begin(), this->collideWith.end(),
                       targetCollider->getParent()->getType()) == 0) {
            return false;
        }

        if (!targetCollider->collideWith.empty() &&
            std::count(targetCollider->collideWith.begin(),
                       targetCollider->collideWith.end(),
                       this->getParent()->getType()) == 0) {
            return false;
        }

        std::array<glm::vec3, 2> rootBB = this->getBoundingBox();
        std::array<glm::vec3, 2> targetBB = targetCollider->getBoundingBox();

        /*std::cout << "--- Comparing ---" << '\n';*/
        /*std::cout << "x: " << rootBB[0].x << " -> " << rootBB[1].x*/
        /*          << " to x: " << targetBB[0].x << " -> " << targetBB[1].x*/
        /*          << '\n';*/
        /*std::cout << "y: " << rootBB[0].y << " -> " << rootBB[1].y*/
        /*          << " to y: " << targetBB[0].y << " -> " << targetBB[1].y*/
        /*          << '\n';*/
        /*std::cout << "z: " << rootBB[0].z << " -> " << rootBB[1].z*/
        /*          << " to z: " << targetBB[0].z << " -> " << targetBB[1].z*/
        /*          << '\n';*/

        bool xAxis =
            rootBB[1].x >= targetBB[0].x && targetBB[1].x >= rootBB[0].x;
        bool yAxis =
            rootBB[1].y >= targetBB[0].y && targetBB[1].y >= rootBB[0].y;
        bool zAxis =
            rootBB[1].z >= targetBB[0].z && targetBB[1].z >= rootBB[0].z;

        bool isColliding = xAxis && yAxis && zAxis;
        if (isColliding) {
            glm::vec3 rootPosition{(rootBB.at(1).x + rootBB.at(0).x) / 2,
                                   (rootBB.at(1).y + rootBB.at(0).y) / 2,
                                   (rootBB.at(1).z + rootBB.at(0).z) / 2};
            glm::vec3 targetPosition{(targetBB.at(1).x + targetBB.at(0).x) / 2,
                                     (targetBB.at(1).y + targetBB.at(0).y) / 2,
                                     (targetBB.at(1).z + targetBB.at(0).z) / 2};

            glm::vec3 targetHalfSize{(targetBB.at(1).x - targetBB.at(0).x) / 2,
                                     (targetBB.at(1).y - targetBB.at(0).y) / 2,
                                     (targetBB.at(1).z - targetBB.at(0).z) / 2};

            glm::vec3 difference = rootPosition - targetPosition;
            glm::vec3 targetClosest =
                targetPosition +
                glm::clamp(difference, -targetHalfSize, targetHalfSize);

            *direction = glm::normalize(rootPosition - targetClosest);
        }

        return isColliding;
    }

    std::array<glm::vec3, 2> getBoundingBox() { return this->boundingBox; }
};

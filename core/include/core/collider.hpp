#pragma once
#include "core/collider_buffer.hpp"
#include "core/object.hpp"
#include "core/shader.hpp"
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
        std::cout << "Creating collider with parent=" << parent << std::endl;
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

    bool isColliding(Collider *targetCollider) {
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

        std::cout << "Collision: "
                  << ((xAxis && yAxis && zAxis) ? "true" : "false") << '\n';

        return xAxis && yAxis && zAxis;
    }

    std::array<glm::vec3, 2> getBoundingBox() { return this->boundingBox; }
};

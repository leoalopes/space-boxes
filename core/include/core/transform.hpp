#pragma once
#include "core/camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 BASE_UP(0.0f, 1.0f, 0.0f);
const glm::vec3 BASE_FORWARD(0.0f, 0.0f, -1.0f);

class Transform {
    glm::vec3 location;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 matrix;

    glm::vec3 forwardVector;
    glm::vec3 rightVector;
    glm::vec3 upVector;

    void updateMatrix() {
        matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, location);
        matrix =
            glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        matrix =
            glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        matrix =
            glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        matrix = glm::scale(matrix, scale);

        this->updateVectors();
    }

    void updateVectors() {
        this->forwardVector =
            glm::normalize(this->matrix * glm::vec4(BASE_FORWARD, 0.0f));
        this->rightVector =
            glm::normalize(glm::cross(this->forwardVector, BASE_UP));
        this->upVector =
            glm::normalize(glm::cross(this->rightVector, this->forwardVector));
    }

  public:
    Transform() : location(0.0f), rotation(0.0f), scale(1.0f) {
        this->updateMatrix();
    };

    glm::mat4 getMatrix() const { return this->matrix; }

    glm::vec3 getLocation() const { return this->location; }
    void setLocation(glm::vec3 newLocation) {
        this->location = newLocation;
        this->updateMatrix();
    }

    glm::vec3 getRotation() const { return this->rotation; }
    void setRotation(glm::vec3 newRotation) {
        this->rotation = newRotation;
        this->updateMatrix();
    }

    glm::vec3 getScale() const { return this->scale; }
    void setScale(glm::vec3 newScale) {
        this->scale = newScale;
        this->updateMatrix();
    }

    glm::vec3 getForwardVector() const { return this->forwardVector; }
    glm::vec3 getRightVector() const { return this->rightVector; }
    glm::vec3 getUpVector() const { return this->upVector; }
};

#include <glad/glad.h>

#include "core/camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 location, glm::vec3 forwardVector, glm::vec3 upVector,
               float fov)
    : forwardVector(forwardVector), fov(fov), location(location),
      worldUp(upVector) {
    this->updateCameraVectors();
}

void Camera::updateCameraVectors() {
    this->rightVector =
        glm::normalize(glm::cross(this->forwardVector, this->worldUp));
    this->upVector =
        glm::normalize(glm::cross(this->rightVector, this->forwardVector));
}

glm::mat4 Camera::getLookAt() {
    return glm::lookAt(this->location, this->location + forwardVector,
                       this->upVector);
}

#include "core/collider.hpp"
#include <core/cube.hpp>

#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

class Player : public Cube {
  private:
    constexpr static const glm::vec2 cameraOffset = glm::vec2(6.0f, 4.0f);
    std::unique_ptr<Collider> collider;

  public:
    using Cube::Cube;

    void addCollider(glm::vec3 dimensions) {
        this->collider = std::make_unique<Collider>(this, dimensions);
    }

    void update() override {
        if (this->scene == nullptr) {
            return;
        }

        GLFWwindow *window = this->scene->getWindow();
        if (window == nullptr) {
            return;
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            std::cout << "Shoot!" << std::endl;
        }

        glm::vec2 movement(0);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            movement.x += 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            movement.x -= 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            movement.y -= 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            movement.y += 1.0f;
        }

        float magnitude =
            sqrt(movement.x * movement.x + movement.y * movement.y);
        if (magnitude > 1) {
            movement.x = movement.x / magnitude;
            movement.y = movement.y / magnitude;
        }
        movement.x = movement.x * 5.0f * this->scene->getDeltaTime();
        movement.y = movement.y * 5.0f * this->scene->getDeltaTime();

        glm::vec3 cubeForwardVector = this->transform.getForwardVector();
        glm::vec3 cubeRightVector = this->transform.getRightVector();
        glm::vec3 location = this->transform.getLocation();
        location += cubeForwardVector * movement.x;
        location += cubeRightVector * movement.y;
        this->transform.setLocation(location);

        glm::vec2 mouseDelta = this->scene->getMouseDelta();
        if (mouseDelta.x != 0.0f) {
            glm::vec3 newRotation = this->transform.getRotation();
            newRotation.y += mouseDelta.x * 0.15f;
            this->transform.setRotation(newRotation);
        }

        Camera *camera = this->scene->getCamera();
        if (camera == nullptr) {
            return;
        }

        glm::vec3 newCameraLocation = location;
        newCameraLocation -=
            this->transform.getForwardVector() * this->cameraOffset.x;
        newCameraLocation +=
            this->transform.getUpVector() * this->cameraOffset.y;

        camera->setLocation(newCameraLocation);
        camera->setForwardVector(location - newCameraLocation);
    }

    void draw() override {
        Cube::draw();

        if (this->collider) {
            this->collider->debug(this->projection, this->view,
                                  this->transform.getMatrix());
        }
    }
};

#pragma once

#include "core/collider.hpp"
#include "core/collision_aware_cube.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

class Player : public Cube, public CollisionAware {
  private:
    constexpr static const glm::vec2 cameraOffset = glm::vec2(6.0f, 4.0f);

  public:
    Player(CubeBuffer buffer, Shader shader, std::vector<Texture> textures)
        : Cube("player", "player", buffer, shader, textures), CollisionAware() {
        this->initializeCollider(this, glm::vec3(-0.5f, -0.5f, -0.5f),
                                 glm::vec3(1.0f, 1.0f, 1.0f));
    }

    void draw(bool isColliding) override {
        Cube::draw(isColliding);

        this->collider->debug(this->projection, this->view,
                              isColliding ? 1 : 0);
    }

    bool isCollisionAware() override { return true; }

    void update(std::vector<Collision> collisions) override {
        if (this->scene == nullptr) {
            return;
        }

        if (collisions.size() > 0) {
            std::cout << "Collisions: ";
            for (int i = 0; i < collisions.size(); i++) {
                if (i != 0) {
                    std::cout << ", ";
                }
                std::cout << collisions.at(i).target->getType();
            }
            std::cout << '\n';

            glm::vec3 location = this->transform.getLocation();
            for (int i = 0; i < collisions.size(); i++) {
                location.x += collisions.at(i).direction.x * 5.0f *
                              this->scene->getDeltaTime();
                location.z += collisions.at(i).direction.z * 5.0f *
                              this->scene->getDeltaTime();
            }
            this->transform.setLocation(location);
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

        std::array<glm::vec3, 2> bb = this->getCollider()->getBoundingBox();
        glm::vec3 beginning = bb[0];
        glm::vec3 end = bb[1];
    }
};

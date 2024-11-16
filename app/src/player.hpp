#pragma once

#include "bullet.hpp"
#include "core/collider.hpp"
#include "core/collision_aware.hpp"
#include "core/cube.hpp"
#include "core/shader.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <vector>

const int shootDelayFrames = 30;

class Player : public Cube, public CollisionAware {
  private:
    constexpr static const glm::vec2 cameraOffset = glm::vec2(6.0f, 4.0f);
    Shader bulletShader;
    std::vector<std::unique_ptr<Bullet>> bullets;
    double shootDelay = 0;

  public:
    Player(CubeBuffer buffer, Shader shader, Shader bulletShader,
           std::vector<Texture> textures)
        : Cube("player", "player", buffer, shader, textures), CollisionAware(),
          bulletShader(bulletShader) {
        this->initializeCollider(this, glm::vec3(-0.5f, -0.5f, -0.5f),
                                 glm::vec3(1.0f, 1.0f, 1.0f));
    }

    void draw(bool isColliding) override {
        if (this->isDestroyed()) {
            return;
        }

        Cube::draw(isColliding);

        this->collider->debug(this->projection, this->view,
                              isColliding ? 1 : 0);
    }

    bool isCollisionAware() override { return true; }

    void update(std::vector<Collision> collisions) override {
        if (this->isDestroyed()) {
            return;
        }

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

        int bulletIndex = 0;
        while (bulletIndex < this->bullets.size()) {
            if (this->bullets.at(bulletIndex)->isDestroyed() &&
                this->bullets.at(bulletIndex)->getScene() == nullptr) {
                this->bullets.at(bulletIndex).reset();
                this->bullets.erase(this->bullets.begin() + bulletIndex);
            } else {
                bulletIndex++;
            }
        }

        if (this->shootDelay > 0) {
            this->shootDelay -= this->scene->getDeltaTime();
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
            this->shootDelay <= 0) {
            std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(
                this->buffer, this->bulletShader, this->textures);

            const glm::vec3 forwardVector = this->transform.getForwardVector();
            const glm::vec3 location = this->transform.getLocation();
            bullet->getTransform()->setLocation(location +
                                                forwardVector * 0.5f);
            bullet->getTransform()->setRotation(this->transform.getRotation());
            bullet->getTransform()->setScale({0.15f, 0.15f, 0.75f});

            this->scene->addObject(bullet.get());

            this->bullets.push_back(std::move(bullet));
            this->shootDelay = shootDelayFrames * this->scene->getDeltaTime();
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

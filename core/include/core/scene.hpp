#pragma once

#include "core/camera.hpp"
#include "core/collision.hpp"
#include "core/object.hpp"
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include <vector>

class Object;

const unsigned int DEFAULT_SCREEN_WIDTH = 1920;
const unsigned int DEFAULT_SCREEN_HEIGHT = 1080;

class Scene {
    GLFWwindow *window;
    Camera *camera;
    std::vector<Object *> objects;
    float aspectRatio;
    float deltaTime;
    glm::vec2 mouseDelta;
    glm::mat4 matrix;
    bool shouldStop;

    /*bool detectionCollision(Object *root, Object *target);*/
    std::map<std::string, std::vector<Collision>> detectCollisions();

  public:
    Scene(unsigned int width = DEFAULT_SCREEN_WIDTH,
          unsigned int height = DEFAULT_SCREEN_HEIGHT);

    std::vector<Object *> getObjects() const { return this->objects; }
    void addObject(Object *object);
    void removeObject(Object *object);

    Camera *getCamera() const { return this->camera; }
    void setCamera(Camera *camera);

    GLFWwindow *getWindow() { return this->window; }
    void setWindow(GLFWwindow *window) { this->window = window; }

    float getDeltaTime() const { return this->deltaTime; }
    void setDeltaTime(float deltaTime) { this->deltaTime = deltaTime; }

    glm::vec2 getMouseDelta() const { return this->mouseDelta; }
    void setMouseDelta(glm::vec2 mouseDelta) { this->mouseDelta = mouseDelta; }

    void draw();

    void stop() { this->shouldStop = true; }
    bool isStopped() { return this->shouldStop; }
};

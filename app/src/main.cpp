#include <glad/glad.h>

#include "core/camera.hpp"
#include "core/cube_buffer.hpp"
#include "core/scene.hpp"
#include "core/shader.hpp"
#include "core/texture.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include "wall.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <stdexcept>

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

bool firstMouseCallback = true;

float mouseX = 0;
float mouseY = 0;
float newMouseX = 0;
float newMouseY = 0;

double fpsLimit = 1.0 / (60);

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xPos, double yPos) {
    if (firstMouseCallback) {
        mouseX = xPos;
        mouseY = yPos;
        newMouseX = xPos;
        newMouseY = yPos;
        firstMouseCallback = false;
        return;
    }

    newMouseX = xPos;
    newMouseY = yPos;
}

GLFWwindow *initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(SCR_WIDTH / 2, SCR_HEIGHT / 2, "SpaceBoxes",
                         glfwGetPrimaryMonitor(), NULL);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glEnable(GL_DEPTH_TEST);

    return window;
}

int main() {
    GLFWwindow *window = initWindow();
    CubeBuffer buffer;

    Shader bulletShader("assets/shaders/cube.vert",
                        "assets/shaders/bullet.frag");
    Shader playerShader("assets/shaders/cube.vert",
                        "assets/shaders/player.frag");
    Texture playerTexture("assets/textures/floor.jpg", GL_RGB);
    Player player(buffer, playerShader, bulletShader, {playerTexture});

    Shader floorShader("assets/shaders/floor.vert",
                       "assets/shaders/floor.frag");
    Texture floorTexture("assets/textures/floor.png", GL_RGB);
    Cube floor("floor", "floor", buffer, floorShader, {floorTexture});
    Transform *floorTransform = floor.getTransform();
    floorTransform->setLocation(glm::vec3(0.0f, -1.0f, 0.0f));
    floorTransform->setScale(glm::vec3(50.0f, 0.5f, 50.0f));

    Wall backWall("backWall", "wall", buffer, floorShader, {floorTexture});
    Transform *backWallTransform = backWall.getTransform();
    backWallTransform->setRotation(glm::vec3(90.0f, 0.0f, 0.0f));
    backWallTransform->setLocation(glm::vec3(0.0f, 3.75f, -25.25f));
    backWallTransform->setScale(glm::vec3(50.0f, 0.5f, 10.0f));

    Wall leftWall("leftWall", "wall", buffer, floorShader, {floorTexture});
    Transform *leftWallTransform = leftWall.getTransform();
    leftWallTransform->setRotation(glm::vec3(90.0f, 0.0f, 90.0f));
    leftWallTransform->setLocation(glm::vec3(25.25f, 3.75f, 0.0f));
    leftWallTransform->setScale(glm::vec3(50.0f, 0.5f, 10.0f));

    Wall rightWall("rightWall", "wall", buffer, floorShader, {floorTexture});
    Transform *rightWallTransform = rightWall.getTransform();
    rightWallTransform->setRotation(glm::vec3(90.0f, 0.0f, 90.0f));
    rightWallTransform->setLocation(glm::vec3(-25.25f, 3.75f, 0.0f));
    rightWallTransform->setScale(glm::vec3(50.0f, 0.5f, 10.0f));

    Wall frontWall("frontWall", "wall", buffer, floorShader, {floorTexture});
    Transform *frontWallTransform = frontWall.getTransform();
    frontWallTransform->setRotation(glm::vec3(90.0f, 0.0f, 0.0f));
    frontWallTransform->setLocation(glm::vec3(0.0f, 3.75f, 25.25f));
    frontWallTransform->setScale(glm::vec3(50.0f, 0.5f, 10.0f));

    std::array<glm::vec3, 2> bounds = {glm::vec3{-24.0f, 0.0f, -24.0f},
                                       glm::vec3{24.0f, 0.0f, 24.0f}};
    Shader enemyShader("assets/shaders/cube.vert", "assets/shaders/enemy.frag");
    Texture enemyTexture("assets/textures/enemy.jpg", GL_RGB);
    std::vector<std::shared_ptr<Enemy>> enemies{};
    float enemySpawnDelay = 0;
    const int enemySpawnDelayFrames = 60;

    Camera camera;
    camera.setLocation(camera.getLocation() + glm::vec3{0.0f, 2.0f, 4.0f});

    Scene scene;
    scene.addObject(&player);
    scene.addObject(&floor);
    scene.addObject(&backWall);
    scene.addObject(&leftWall);
    scene.addObject(&rightWall);
    scene.addObject(&frontWall);
    scene.setCamera(&camera);
    scene.setWindow(window);

    double previousFrame = glfwGetTime();
    double previousRender = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        double deltaTime = currentFrame - previousFrame;
        previousFrame = currentFrame;

        if ((currentFrame - previousRender) < fpsLimit) {
            continue;
        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
            scene.isStopped()) {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(0.502, 0.502, 0.502, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float mouseDeltaX = mouseX - newMouseX;
        float mouseDeltaY = newMouseY - mouseY;
        mouseX = newMouseX;
        mouseY = newMouseY;

        if (enemySpawnDelay <= 0) {
            auto enemy =
                std::make_shared<Enemy>(bounds, &player, buffer, enemyShader,
                                        std::vector<Texture>{enemyTexture});
            enemies.push_back(enemy);
            scene.addObject(enemy.get());
            enemySpawnDelay = 120 * deltaTime;
        } else {
            enemySpawnDelay -= deltaTime;
        }

        player.getCollider()->updateBoundingBox();
        frontWall.getCollider()->updateBoundingBox();
        backWall.getCollider()->updateBoundingBox();
        leftWall.getCollider()->updateBoundingBox();
        rightWall.getCollider()->updateBoundingBox();

        scene.setMouseDelta(glm::vec2(mouseDeltaX, mouseDeltaY));
        scene.setDeltaTime(deltaTime);
        scene.draw();

        int enemyIndex = 0;
        while (enemyIndex < enemies.size()) {
            if (enemies.at(enemyIndex)->isDestroyed() &&
                enemies.at(enemyIndex)->getScene() == nullptr) {
                enemies.at(enemyIndex).reset();
                enemies.erase(enemies.begin() + enemyIndex);
            } else {
                enemyIndex++;
            }
        }

        glfwPollEvents();
        glfwSwapBuffers(window);

        mouseDeltaX = 0.0f;
        mouseDeltaY = 0.0f;
    }

    glfwTerminate();
    return 0;
}

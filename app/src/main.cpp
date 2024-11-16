#include <glad/glad.h>

#include "core/camera.hpp"
#include "core/cube_buffer.hpp"
#include "core/scene.hpp"
#include "core/shader.hpp"
#include "core/texture.hpp"
#include "player.hpp"
#include "wall.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

bool firstMouseCallback = true;

float mouseX = 0;
float mouseY = 0;
float newMouseX = 0;
float newMouseY = 0;

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

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH / 2, SCR_HEIGHT / 2,
                                          "SpaceBoxes", NULL, NULL);
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
    Shader cubeShader("assets/shaders/cube.vert", "assets/shaders/cube.frag");
    Texture cubeTexture("assets/textures/floor.jpg", GL_RGB);
    Player player(buffer, cubeShader, bulletShader, {cubeTexture});

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
    backWallTransform->setLocation(glm::vec3(0.0f, 1.25f, -25.25f));
    backWallTransform->setScale(glm::vec3(50.0f, 0.5f, 5.0f));

    Wall leftWall("leftWall", "wall", buffer, floorShader, {floorTexture});
    Transform *leftWallTransform = leftWall.getTransform();
    leftWallTransform->setRotation(glm::vec3(90.0f, 0.0f, 90.0f));
    leftWallTransform->setLocation(glm::vec3(25.25f, 1.25f, 0.0f));
    leftWallTransform->setScale(glm::vec3(50.0f, 0.5f, 5.0f));

    Wall rightWall("rightWall", "wall", buffer, floorShader, {floorTexture});
    Transform *rightWallTransform = rightWall.getTransform();
    rightWallTransform->setRotation(glm::vec3(90.0f, 0.0f, 90.0f));
    rightWallTransform->setLocation(glm::vec3(-25.25f, 1.25f, 0.0f));
    rightWallTransform->setScale(glm::vec3(50.0f, 0.5f, 5.0f));

    Wall frontWall("frontWall", "wall", buffer, floorShader, {floorTexture});
    Transform *frontWallTransform = frontWall.getTransform();
    frontWallTransform->setRotation(glm::vec3(90.0f, 0.0f, 0.0f));
    frontWallTransform->setLocation(glm::vec3(0.0f, 1.25f, 25.25f));
    frontWallTransform->setScale(glm::vec3(50.0f, 0.5f, 5.0f));

    Texture enemyTexture("assets/textures/enemy.jpg", GL_RGB);
    CollisionAwareCube enemy("enemy", "enemy", buffer, cubeShader,
                             {enemyTexture});
    Transform *enemyTransform = enemy.getTransform();
    enemyTransform->setLocation(glm::vec3(4.0f, 0.0f, -6.0f));

    Camera camera;

    Scene scene;
    scene.addObject(&player);
    scene.addObject(&floor);
    scene.addObject(&backWall);
    scene.addObject(&leftWall);
    scene.addObject(&rightWall);
    scene.addObject(&frontWall);
    scene.addObject(&enemy);
    scene.setCamera(&camera);
    scene.setWindow(window);

    double previousFrame = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        double deltaTime = currentFrame - previousFrame;
        previousFrame = currentFrame;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float mouseDeltaX = mouseX - newMouseX;
        float mouseDeltaY = newMouseY - mouseY;
        mouseX = newMouseX;
        mouseY = newMouseY;

        enemy.getCollider()->updateBoundingBox();
        player.getCollider()->updateBoundingBox();
        frontWall.getCollider()->updateBoundingBox();
        backWall.getCollider()->updateBoundingBox();
        leftWall.getCollider()->updateBoundingBox();
        rightWall.getCollider()->updateBoundingBox();

        scene.setMouseDelta(glm::vec2(mouseDeltaX, mouseDeltaY));
        scene.setDeltaTime(deltaTime);
        scene.draw();

        glfwPollEvents();
        glfwSwapBuffers(window);

        mouseDeltaX = 0.0f;
        mouseDeltaY = 0.0f;
    }

    glfwTerminate();
    return 0;
}

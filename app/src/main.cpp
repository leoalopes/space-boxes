#include <glad/glad.h>

#include "core/camera.hpp"
#include "core/cube.hpp"
#include "core/cube_buffer.hpp"
#include "core/scene.hpp"
#include "core/shader.hpp"
#include "core/texture.hpp"
#include "player.hpp"
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

    Shader cubeShader("assets/shaders/cube.vert", "assets/shaders/cube.frag");
    Texture cubeTexture("assets/textures/floor.jpg", GL_RGB);
    Player cube(buffer, cubeShader, {cubeTexture});
    cube.addCollider(glm::vec3(1.0f, 1.0f, 1.0f));
    Transform *cubeTransform = cube.getTransform();
    glm::vec2 cameraOffset(-6.0f, -5.0f);

    Shader floorShader("assets/shaders/floor.vert",
                       "assets/shaders/floor.frag");
    Texture floorTexture("assets/textures/floor.png", GL_RGB);
    Cube floor(buffer, floorShader, {floorTexture});
    Transform *floorTransform = floor.getTransform();
    floorTransform->setLocation(glm::vec3(0.0f, -1.0f, 0.0f));
    floorTransform->setScale(glm::vec3(20.0f, 0.5f, 20.0f));

    Texture enemyTexture("assets/textures/enemy.jpg", GL_RGB);
    Cube enemy(buffer, cubeShader, {enemyTexture});
    Transform *enemyTransform = enemy.getTransform();
    enemyTransform->setLocation(glm::vec3(4.0f, 0.0f, -6.0f));

    Camera camera;

    Scene scene;
    scene.addObject(&cube);
    scene.addObject(&floor);
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

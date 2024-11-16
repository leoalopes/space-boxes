#include <glad/glad.h>

#include "core/cube.hpp"
#include "core/cube_buffer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Cube::Cube(std::string name, std::string type, CubeBuffer buffer, Shader shader,
           std::vector<Texture> textures)
    : Object(name, type), shader(shader), textures(textures) {
    this->shader.use();
    for (int tex = 0; tex < this->textures.size(); tex++) {
        shader.setInt("texture" + std::to_string(tex + 1), tex);
    }
}

void Cube::draw(bool isColliding) {
    if (this->isDestroyed()) {
        return;
    }

    for (int tex = 0; tex < this->textures.size(); tex++) {
        glActiveTexture(GL_TEXTURE0 + tex);
        this->textures.at(tex).use();
    }

    glm::mat4 model = this->transform.getMatrix();
    this->shader.use();
    this->shader.setMatrix("model", glm::value_ptr(model));

    this->buffer.draw();
}

void Cube::setViewMatrix(glm::mat4 matrix) {
    Object::setViewMatrix(matrix);
    this->shader.use();
    this->shader.setMatrix("view", glm::value_ptr(matrix));
}

void Cube::setProjectionMatrix(glm::mat4 matrix) {
    Object::setProjectionMatrix(matrix);
    this->shader.use();
    this->shader.setMatrix("projection", glm::value_ptr(matrix));
}

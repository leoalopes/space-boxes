#pragma once
#include <glad/glad.h>

#include "core/cube_buffer.hpp"
#include "core/object.hpp"
#include "core/shader.hpp"
#include "core/texture.hpp"
#include <vector>

class Cube : public Object {
  protected:
    std::vector<Texture> textures;
    Shader shader;
    CubeBuffer buffer;

  public:
    Cube(std::string name, std::string type, CubeBuffer buffer, Shader shader,
         std::vector<Texture> textures);

    void draw(bool isColliding) override;

    void setViewMatrix(glm::mat4 matrix) override;
    void setProjectionMatrix(glm::mat4 matrix) override;

    ObjectBuffer *getBuffer() override { return &this->buffer; }
};

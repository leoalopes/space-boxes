#pragma once
#include <glad/glad.h>

class Texture {
    unsigned int textureId;

  public:
    Texture(const char *imagePath, GLenum type);

    void use();
};

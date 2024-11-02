#pragma once
#include <string>

class Shader {
    unsigned int programId;

  public:
    Shader(const char *vertexPath, const char *fragmentPath);

    void use();

    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMatrix(const std::string &name, float *value) const;
};

#pragma once

#include <glad/glad.h>

#include <string>

class Shader
{
public:
    explicit Shader(const std::string& filename);
    ~Shader();

public:
    void bind();

private:
    static const unsigned int NUM_SHADERS_ = 2;
    GLuint                    program_     = 0;
    GLuint                    shaders_[NUM_SHADERS_];
};

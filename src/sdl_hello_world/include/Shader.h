#pragma once

#include "Transform.h"

#include <glad/glad.h>

#include <string>

class Shader
{
public:
    explicit Shader(const std::string& filename);
    ~Shader();

public:
    void bind();

    void update(const Transform& transform);

private:
    static const unsigned int NUM_SHADERS_ = 2;

    enum
    {
        TRNASFORM_U,
        NUM_UNIFORMS_
    };
    GLuint program_ = 0;
    GLuint shaders_[NUM_SHADERS_];
    GLuint uniforms_[NUM_UNIFORMS_];
};

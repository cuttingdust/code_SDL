#pragma once

#include "Camera.h"
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

    void update(const Transform& transform, const Camera& camera);

private:
    static const unsigned int NUM_SHADERS_ = 2;

    enum
    {
        TRANSFORM_U,
        NUM_UNIFORMS_
    };
    GLuint program_ = 0;
    GLuint shaders_[NUM_SHADERS_];
    GLuint uniforms_[NUM_UNIFORMS_];
};

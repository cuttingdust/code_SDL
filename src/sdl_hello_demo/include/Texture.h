#pragma once

#include <string>

#include <glad/glad.h>

class Texture
{
public:
    Texture(const std::string& fname);
    virtual ~Texture();
public:
    void bind();

private:
    GLuint textureID_ = 0;
};

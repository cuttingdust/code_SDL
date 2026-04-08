#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

Texture::Texture(const std::string &fname)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(fname.c_str(), &width, &height, &channels, 4);
    if (!data)
    {
        throw std::runtime_error("Failed to load texture: " + fname);
    }
    // Here you would typically create an OpenGL texture and upload the data
    // For example:
    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

Texture::~Texture()
{
    if (textureID_ != 0)
    {
        glDeleteTextures(1, &textureID_);
    }
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, textureID_);
}

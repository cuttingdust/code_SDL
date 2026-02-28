#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

class Vertex
{
public:
    Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
    {
        position_ = pos;
        texCoord_ = texCoord;
    }

    inline glm::vec3* getPos()
    {
        return &position_;
    }

    inline glm::vec2* getTexCoord()
    {
        return &texCoord_;
    }

private:
    glm::vec3 position_;
    glm::vec2 texCoord_;
};


class Mesh
{
public:
    Mesh(Vertex* vert, unsigned int numVertices);
    ~Mesh();

public:
    void draw();

private:
    enum
    {
        POSITION_VB,
        TEXCOORD_VB,
        NUM_BUFFERS
    };

    GLuint       vertexArrayObject_               = 0;
    GLuint       vertexArrayBuffers_[NUM_BUFFERS] = { 0 };
    unsigned int drawCount_                       = 0;
};

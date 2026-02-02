#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

class Vertex
{
public:
    Vertex(const glm::vec3& pos)
    {
        position_ = pos;
    }

private:
    glm::vec3 position_;
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
        NUM_BUFFERS
    };

    GLuint       vertexArrayObject_               = 0;
    GLuint       vertexArrayBuffers_[NUM_BUFFERS] = { 0 };
    unsigned int drawCount_                       = 0;
};

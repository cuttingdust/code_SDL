#include "Mesh.h"

Mesh::Mesh(Vertex *vert, unsigned int numVertices)
{
    drawCount_ = numVertices;
    glGenVertexArrays(1, &vertexArrayObject_);
    glBindVertexArray(vertexArrayObject_);

    glGenBuffers(NUM_BUFFERS, vertexArrayBuffers_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers_[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vert[0]), vert, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vert[0]), 0);


    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(NUM_BUFFERS, vertexArrayBuffers_);
    glDeleteVertexArrays(1, &vertexArrayObject_);
}

void Mesh::draw()
{
    glBindVertexArray(vertexArrayObject_);
    glDrawArrays(GL_TRIANGLES, 0, drawCount_);
    glBindVertexArray(0);
}

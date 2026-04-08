#include "Mesh.h"

Mesh::Mesh(Vertex *vert, unsigned int numVertices)
{
    drawCount_ = numVertices;
    glGenVertexArrays(1, &vertexArrayObject_);
    glBindVertexArray(vertexArrayObject_);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        positions.push_back(*vert[i].getPos());
        texCoords.push_back(*vert[i].getTexCoord());
    }

    glGenBuffers(NUM_BUFFERS, vertexArrayBuffers_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers_[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //////////////////////////////////////////////////////////////////

    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers_[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);


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

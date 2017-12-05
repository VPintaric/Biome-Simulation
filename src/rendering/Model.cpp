#include <GL/glew.h>
#include "../state/Log.h"

#include "Model.h"

Model::Model(const std::vector<GLfloat> &vs) : vertices(vs) {
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glGenBuffers(1, &verticesVboId);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVboId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

Model::~Model() {
    glDeleteBuffers(1, &verticesVboId);
    glDeleteVertexArrays(1, &vaoId);
}

void Model::draw() {
    glBindVertexArray(vaoId);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

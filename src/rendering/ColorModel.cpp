#include <GL/glew.h>
#include "state/Log.h"

#include "rendering/ColorModel.h"

ColorModel::ColorModel(const std::vector<GLfloat>& vs, const std::vector<GLfloat>& cs,
                       GLenum primitiveType, GLenum colorBufferUsage)
    : Model(vs, primitiveType), colors(cs) {
    
    glGenBuffers(1, &colorsVboId);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), &colors[0], colorBufferUsage);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
}


ColorModel::~ColorModel() {
    glDeleteBuffers(1, &colorsVboId);
}

void ColorModel::draw() {
    Model::draw();
}

void ColorModel::updateColorBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);
    glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(GLfloat), &colors[0]);
}
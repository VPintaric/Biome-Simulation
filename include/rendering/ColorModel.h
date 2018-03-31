#ifndef COLORMODEL_H
#define COLORMODEL_H

#include "Model.h"


class ColorModel : public Model {
public:
    explicit ColorModel(const std::vector<GLfloat> &vs, const std::vector<GLfloat> &cs,
                        GLenum primitiveType = GL_TRIANGLES, GLenum colorBufferUsage = GL_STATIC_DRAW);
    virtual ~ColorModel();
    virtual void draw();
    virtual void updateColorBuffer();

    std::vector<GLfloat> colors;

private:
    GLuint colorsVboId;
};

#endif /* COLORMODEL_H */


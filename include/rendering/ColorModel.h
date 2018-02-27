#ifndef COLORMODEL_H
#define COLORMODEL_H

#include "Model.h"


class ColorModel : public Model {
public:
    ColorModel(const std::vector<GLfloat> &vs, const std::vector<GLfloat> &cs);
    virtual ~ColorModel();
    virtual void draw();
    
private:
    GLuint colorsVboId;
    std::vector<GLfloat> colors;
};

#endif /* COLORMODEL_H */


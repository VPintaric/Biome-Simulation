#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <SDL2/SDL_opengl.h>

class Model {
public:
    Model(const std::vector<GLfloat> &vs);
    virtual ~Model();

    virtual void draw();
    
protected:
    GLuint vaoId;
    GLuint verticesVboId;
    std::vector<GLfloat> vertices;

};

#endif // MODEL_H

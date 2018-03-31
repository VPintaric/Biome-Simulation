#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <SDL2/SDL_opengl.h>

class Model {
public:
    explicit Model(const std::vector<GLfloat> &vs, GLenum primitiveType = GL_TRIANGLES);
    virtual ~Model();

    virtual void draw();
    
protected:
    GLuint vaoId;
    GLuint verticesVboId;
    std::vector<GLfloat> vertices;

    GLenum primitiveType;
};

#endif // MODEL_H

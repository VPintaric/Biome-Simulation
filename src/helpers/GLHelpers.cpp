#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "../state/Log.h"

#include "GLHelpers.h"

bool GLHelpers::checkGLError(){
    GLenum err = glGetError();
    if(err != GL_NO_ERROR){
        Log().Get(logERROR) << "GL ERROR CODE: " << err;
        return true;
    }
    return false;
}
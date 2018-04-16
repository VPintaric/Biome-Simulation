#include <GL/glew.h>
#include "state/Log.h"

#include "helpers/GLHelpers.h"

bool GLHelpers::checkGLError(){
    GLenum err = glGetError();
    if(err != GL_NO_ERROR){
        Log().Get(logERROR) << "GL ERROR CODE: " << err;
        return true;
    }
    return false;
}
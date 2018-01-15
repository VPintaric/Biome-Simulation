#include <vector>
#include <GL/glew.h>
#include <memory>

#include "../state/Log.h"
#include "ProgramLoader.h"

ProgramLoader::ProgramLoader(std::string vertexShaderSourceFile, std::string fragmentShaderSourceFile) {
    std::unique_ptr<ShaderLoader> vertexShader = std::unique_ptr<ShaderLoader>(new ShaderLoader(GL_VERTEX_SHADER, vertexShaderSourceFile));
    std::unique_ptr<ShaderLoader> fragmentShader = std::unique_ptr<ShaderLoader>(new ShaderLoader(GL_FRAGMENT_SHADER, fragmentShaderSourceFile));
    
    if(!vertexShader->isUseable() || !fragmentShader->isUseable()){
        Log().Get(logWARNING) << "At least one of the given shader programs didn't compile successfully...";
        isUseableFlag = false;
        return;
    }
    
    programId = glCreateProgram();
    if(programId == 0){
        Log().Get(logWARNING) << "OpenGL couldn't create a new shader program...";
        isUseableFlag = false;
        return;
    }
    
    glAttachShader(programId, vertexShader->getShaderId());
    glAttachShader(programId, fragmentShader->getShaderId());
    
    glLinkProgram(programId);
    
    GLint compilationFlag;
    glGetProgramiv(programId, GL_LINK_STATUS, &compilationFlag);
    if(compilationFlag == GL_FALSE){
        int infoLogLength;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        
        std::vector<char> linkingErrLog(infoLogLength + 1);
        glGetProgramInfoLog(programId, infoLogLength, NULL, &linkingErrLog[0]);
        Log().Get(logWARNING) << "Error during shader program linking...";
        Log().Get(logWARNING) << &linkingErrLog[0];
        
        isUseableFlag = false;
        return;
    }
    
    isUseableFlag = true;
    Log().Get(logDEBUG) << "Shader program linking successful, programId = " << programId;
    
    glDetachShader(programId, vertexShader->getShaderId());
    glDetachShader(programId, fragmentShader->getShaderId());
}


ProgramLoader::ProgramLoader(const ProgramLoader& orig) {
}

ProgramLoader::~ProgramLoader() {
}

GLuint ProgramLoader::getId() {
    return programId;
}

bool ProgramLoader::isUseable() {
    return isUseableFlag;
}

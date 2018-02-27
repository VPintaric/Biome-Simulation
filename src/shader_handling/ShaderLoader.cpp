#include <fstream>
#include <vector>
#include <GL/glew.h>

#include "state/Log.h"
#include "shader_handling/ShaderLoader.h"

ShaderLoader::ShaderLoader(GLuint shaderType, std::string fileName) {
    this->fileName = fileName;
    this->shaderType = shaderType;
    
    Log().Get(logINFO) << "Reading source code for shader in file path: " << fileName;
    std::ifstream sourceStream(fileName);
    if(!sourceStream.is_open()){
        isUseableFlag = false;
        Log().Get(logWARNING) << "Cannot open given file for shader loading: " << fileName;
        return;
    }
    
    std::string line = "";
    while(getline(sourceStream, line)){
        shaderSourceCode += line + "\n";
    }
    sourceStream.close();
    
    Log().Get(logDEBUG) << "Creating new shader object...";
    shaderId = glCreateShader(shaderType);
    if(shaderId == 0){
        isUseableFlag = false;
        Log().Get(logWARNING) << "OpenGL could not create a new shader object";
        return;
    }
    Log().Get(logDEBUG) << "Created new shader object, id = " << shaderId;
    
    Log().Get(logDEBUG) << "Compiling read shader source code...";
    char const * sourceRawP = shaderSourceCode.c_str();
    glShaderSource(shaderId, 1, &sourceRawP, nullptr);
    glCompileShader(shaderId);
    
    GLint compilationFlag;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilationFlag);
    if(compilationFlag == GL_FALSE){
        int infoLogLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        
        std::vector<char> compilationErrMsg(infoLogLength + 1);
        glGetShaderInfoLog(shaderId, infoLogLength, nullptr, &compilationErrMsg[0]);
        Log().Get(logWARNING) << "Error while compiling shader...";
        Log().Get(logWARNING) << &compilationErrMsg[0];
        
        isUseableFlag = false;
        return;
    }
    
    Log().Get(logDEBUG) << "Compilation successful";
    isUseableFlag = true;
}


ShaderLoader::ShaderLoader(const ShaderLoader& orig) {
}

ShaderLoader::~ShaderLoader() {
    if(shaderId != 0){
        Log().Get(logDEBUG) << "Deleting shader (id = " << shaderId << ") compiled from file: " << fileName;
        glDeleteShader(shaderId);
    }
}

const std::string& ShaderLoader::getFileName() {
    return fileName;
}

GLuint ShaderLoader::getShaderId() {
    return shaderId;
}

GLuint ShaderLoader::getShaderType() {
    return shaderType;
}

const std::string& ShaderLoader::getSourceCode() {
    return shaderSourceCode;
}

bool ShaderLoader::isUseable() {
    return isUseableFlag;
}

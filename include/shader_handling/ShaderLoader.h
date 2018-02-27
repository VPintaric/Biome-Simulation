#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <string>


class ShaderLoader {
public:
    ShaderLoader(GLuint shaderType, std::string fileName);
    ShaderLoader(const ShaderLoader& orig);
    virtual ~ShaderLoader();
    
    const std::string& getSourceCode();
    const std::string& getFileName();
    GLuint getShaderType();
    GLuint getShaderId();
    bool isUseable();
private:
    
    std::string shaderSourceCode, fileName;
    GLuint shaderType, shaderId;
    bool isUseableFlag;
};

#endif /* SHADERLOADER_H */


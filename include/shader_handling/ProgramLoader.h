#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H

#include <string>
#include <memory>

#include "ShaderLoader.h"

class ProgramLoader {
public:
    ProgramLoader(std::string vertexShaderSourceFile, std::string fragmentShaderSourceFile);
    ProgramLoader(const ProgramLoader& orig);
    virtual ~ProgramLoader();
    
    GLuint getId();
    bool isUseable();
private:

    GLuint programId;
    bool isUseableFlag;
};

#endif /* PROGRAMLOADER_H */


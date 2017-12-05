#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include <string>
#include <memory>
#include "Model.h"
#include "../shader_handling/ProgramLoader.h"


class Renderer {
public:
    static Renderer& getInstance();
    virtual ~Renderer();
    
    void addNewModel(std::string name, std::shared_ptr<Model> model);
    std::shared_ptr<Model> getModel(std::string name);
    void removeModel(std::string name);
    
private:
    Renderer();
    std::map<std::string, std::shared_ptr<Model> > models;
};

#endif /* RENDERER_H */


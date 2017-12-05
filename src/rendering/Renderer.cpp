#include "../state/Log.h"
#include "Renderer.h"

Renderer& Renderer::getInstance(){
    static Renderer instance;
    return instance;
}

Renderer::Renderer() {
    Log().Get(logDEBUG) << "Creating an instance of renderer";
}

Renderer::~Renderer() {
    Log().Get(logDEBUG) << "Destroying renderer instance";
}

void Renderer::addNewModel(std::string name, std::shared_ptr<Model> model) {
    Log().Get(logDEBUG) << "Adding new model with name: \"" << name << "\" to models container";
    
    std::map<std::string, std::shared_ptr<Model> >::iterator iter = models.find(name);
    if(iter != models.end()){
        Log().Get(logWARNING) << "Model with name \"" << name << "\" already exists";
        return;
    }
    
    models.insert(std::pair<std::string, std::shared_ptr<Model> >(name, model));
}

std::shared_ptr<Model> Renderer::getModel(std::string name) {
    std::map<std::string, std::shared_ptr<Model> >::iterator iter = models.find(name);
    
    if(iter == models.end()){
        Log().Get(logWARNING) << "Model with name \"" << name << "\" does not exist, returning null";
        return nullptr;
    }
    
    return iter->second;
}

void Renderer::removeModel(std::string name) {
    Log().Get(logDEBUG) << "Removing model with name \"" << name << "\"";
    
    std::map<std::string, std::shared_ptr<Model> >::iterator iter = models.find(name);
    
    if(iter == models.end()){
        Log().Get(logWARNING) << "Model with name \"" << name << "\" does not exist";
        return;
    }
    
    models.erase(iter);
}

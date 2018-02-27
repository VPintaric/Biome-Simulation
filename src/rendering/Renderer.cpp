#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "state/Log.h"
#include "rendering/Renderer.h"

Renderer& Renderer::getInstance(){
    static Renderer instance;
    return instance;
}

Renderer::Renderer() {
    Log().Get(logDEBUG) << "Creating an instance of renderer";
    projection = view = model = glm::mat4(1.f);
}

Renderer::~Renderer() {
    Log().Get(logDEBUG) << "Destroying renderer instance";
}

void Renderer::addNewModel(std::string name, std::shared_ptr<Model> model) {
    Log().Get(logDEBUG) << "Adding new model with name: \"" << name << "\" to models container";

    auto iter = models.find(name);
    if(iter != models.end()){
        Log().Get(logWARNING) << "Model with name \"" << name << "\" already exists";
        return;
    }
    
    models.insert(std::make_pair(name, model));
}

std::shared_ptr<Model> Renderer::getModel(std::string name) {
    auto iter = models.find(name);
    
    if(iter == models.end()){
        Log().Get(logWARNING) << "Model with name \"" << name << "\" does not exist, returning null";
        return nullptr;
    }
    
    return iter->second;
}

void Renderer::removeModel(std::string name) {
    Log().Get(logDEBUG) << "Removing model with name \"" << name << "\"";

    auto iter = models.find(name);
    
    if(iter == models.end()){
        Log().Get(logWARNING) << "Model with name \"" << name << "\" does not exist";
        return;
    }
    
    models.erase(iter);
}

void Renderer::addShaderProgram(std::string name, std::shared_ptr<ProgramLoader> shader) {
    Log().Get(logDEBUG) << "Adding shader program to renderer: \"" << name << "\"";

    auto iter = shaderPrograms.find(name);
    
    if(iter != shaderPrograms.end()){
        Log().Get(logWARNING) << "Shader program with name \"" << name << "\" already exists";
        return;
    }
    
    shaderPrograms.insert(std::pair<std::string, std::shared_ptr<ProgramLoader> >(name, shader));
}

std::shared_ptr<ProgramLoader> Renderer::getShaderProgram(std::string name) {
    auto iter = shaderPrograms.find(name);
    
    if(iter == shaderPrograms.end()){
        Log().Get(logWARNING) << "Shader program with name \"" << name << "\" does not exist, returning null";
        return nullptr;
    }
    
    return iter->second;
}

std::shared_ptr<ProgramLoader> Renderer::getActiveShaderProgram() {
    return activeShader;
}


void Renderer::removeShaderProgram(std::string name) {
    Log().Get(logDEBUG) << "Removing shader program with name \"" << name << "\"";

    auto iter = shaderPrograms.find(name);
    
    if(iter == shaderPrograms.end()){
        Log().Get(logWARNING) << "Shader program with name \"" << name << "\" does not exist";
        return;
    }
    
    shaderPrograms.erase(iter);
}

void Renderer::activateShaderProgram(std::string name) {
    Log().Get(logDEBUG) << "Activating shader program \"" << name << "\"";
    
    std::shared_ptr<ProgramLoader> shader = getShaderProgram(name);
    
    if(shader == nullptr){
        Log().Get(logWARNING) << "Can't activate shader program \"" << name << "\"";
        return;
    }
    
    GLuint shaderId = shader->getId();
    glUseProgram(shaderId);
    activeShader = shader;
    updateAllShaderMatrices();
}

void Renderer::pushMatrix(Matrix t) {
    switch(t){
        case Matrix::VIEW:
            viewStack.push_back(view);
            break;
        case Matrix::PROJECTION:
            projectionStack.push_back(projection);
            break;
        case Matrix::MODEL:
            modelStack.push_back(model);
            break;
    }
}

void Renderer::popMatrix(Matrix t) {
    switch(t){
        case Matrix::VIEW:
            if(viewStack.empty()){
                Log().Get(logWARNING) << "View matrix stack is empty, can't pop";
                break;
            }
            view = viewStack.back();
            viewStack.pop_back();
            break;
        case Matrix::PROJECTION:
            if(projectionStack.empty()){
                Log().Get(logWARNING) << "Projection matrix stack is empty, can't pop";
                break;
            }
            projection = projectionStack.back();
            projectionStack.pop_back();
            break;
        case Matrix::MODEL:
            if(modelStack.empty()){
                Log().Get(logWARNING) << "Model matrix stack is empty, can't pop";
                break;
            }
            model = modelStack.back();
            modelStack.pop_back();
            break;
    }
}

void Renderer::transform(float translX, float translY, float translZ, float rotAngle, float scaleX, float scaleY) {
    model = 
        glm::scale(
            glm::rotate(
                glm::translate(model, glm::vec3(-translX, -translY, translZ)), 
                rotAngle, glm::vec3(0., 0., 1.)),
            glm::vec3(scaleX, scaleY, 1.));
    
    updateShaderMatrix(Matrix::MODEL);
}

void Renderer::translate(float x, float y, float z) {
    model = glm::translate(model, glm::vec3(-x, -y, z));
    updateShaderMatrix(Matrix::MODEL);
}

void Renderer::rotate(float angle) {
    model = glm::rotate(model, angle, glm::vec3(0., 0., 1.));
    updateShaderMatrix(Matrix::MODEL);
}

void Renderer::scale(float x, float y) {
    model = glm::scale(model, glm::vec3(x, y, 1.));
    updateShaderMatrix(Matrix::MODEL);
}

void Renderer::identity() {
    model = glm::mat4(1.f);
    updateShaderMatrix(Matrix::MODEL);
}


void Renderer::setOrthoProjection(float left, float right, float bottom, float top) {
    projection = glm::ortho(left, right, bottom, top, MIN_Z, MAX_Z);
    updateShaderMatrix(Matrix::PROJECTION);
}

void Renderer::setPerspectiveProjection(float fovy, float aspect, float near) {
    projection = glm::infinitePerspective(fovy, aspect, near);
    updateShaderMatrix(Matrix::PROJECTION);
}

void Renderer::setCameraPosition(float x, float y, float z) {
    glm::vec3 pos = glm::vec3(x, y, z);
    glm::vec3 poi = pos + cameraOrient;
    view = glm::lookAt(pos, poi, cameraUp);
    updateShaderMatrix(Matrix::VIEW);
}

void Renderer::updateShaderMatrix(Matrix t) {
    GLuint shaderId = activeShader->getId();
    
    switch(t){
        case Matrix::PROJECTION:
            glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            break;
        case Matrix::VIEW:
            glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, GL_FALSE, glm::value_ptr(view));
            break;
        case Matrix::MODEL:
            glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model));
            break;
    }
}

void Renderer::updateAllShaderMatrices() {
    GLuint shaderId = activeShader->getId();
    
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

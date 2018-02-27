#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include <string>
#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include "Model.h"
#include "shader_handling/ProgramLoader.h"


class Renderer {
public:
    const float MAX_Z = 100.f;
    const float MIN_Z = -100.f;
    
    enum class Matrix {
        PROJECTION, VIEW, MODEL
    };
    
    static Renderer& getInstance();
    virtual ~Renderer();
    
    void addNewModel(std::string name, std::shared_ptr<Model> model);
    std::shared_ptr<Model> getModel(std::string name);
    void removeModel(std::string name);
    
    void addShaderProgram(std::string name, std::shared_ptr<ProgramLoader> shader);
    std::shared_ptr<ProgramLoader> getShaderProgram(std::string name);
    std::shared_ptr<ProgramLoader> getActiveShaderProgram();
    void removeShaderProgram(std::string name);
    void activateShaderProgram(std::string name);
    
    void pushMatrix(Matrix t);
    void popMatrix(Matrix t);
    
    void transform(float translX, float translY, float translZ, float rotAngle, float scaleX, float scaleY);
    void translate(float x, float y, float z = 0.f);
    void rotate(float angle);
    void scale(float x, float y);
    void identity();
    void setOrthoProjection(float left, float right, float bottom, float top);
    void setPerspectiveProjection(float fovy, float aspect, float near);
    void setCameraPosition(float x, float y, float z = -1.f);
    
private:
    Renderer();

    void updateShaderMatrix(Matrix t);
    void updateAllShaderMatrices();
    
    std::map<std::string, std::shared_ptr<Model> > models;
    std::map< std::string, std::shared_ptr<ProgramLoader> > shaderPrograms;
    
    std::shared_ptr<ProgramLoader> activeShader;
    
    std::vector< glm::mat4 > projectionStack, viewStack, modelStack;
    glm::mat4 projection, view, model;
    
    const glm::vec3 cameraOrient = glm::vec3(0., 0., 1.);
    const glm::vec3 cameraUp = glm::vec3(0., 1., 0.);
};

#endif /* RENDERER_H */


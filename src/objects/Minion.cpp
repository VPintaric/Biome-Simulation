#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include "Minion.h"
#include "../rendering/Renderer.h"
#include "../state/Log.h"
#include "../constants/SimulationConstants.h"

#include <algorithm>
#include <GL/glm/gtc/quaternion.hpp>
#include <GL/glm/gtx/quaternion.hpp>
#include <GL/glm/gtx/vector_angle.hpp>
#include <GL/glm/gtc/type_ptr.hpp>

Minion::Minion() {
    Renderer &r = Renderer::getInstance();
    bodyModel = r.getModel(SimConst::MINION_BODY_MODEL_NAME);
    circleModel = r.getModel(SimConst::MINION_CIRCLE_MODEL_NAME);
}

Minion::~Minion() {
}

void Minion::draw() const {
    glm::vec2 pos = getPos();
    float angle = getAngle();
    float scale = getRadius();

    Renderer &r = Renderer::getInstance();
    
    GLuint shaderId = r.getActiveShaderProgram()->getId();
    glUniform4fv(glGetUniformLocation(shaderId, "color_mod"), 1, glm::value_ptr(color));

    r.pushMatrix(Renderer::Matrix::MODEL);
    r.transform(pos.x, pos.y, 0.f,
            angle, scale, scale);
    bodyModel->draw();
    circleModel->draw();
    r.popMatrix(Renderer::Matrix::MODEL);
}

void Minion::update(float deltaT) {
    CircleObject::update(deltaT);
}

glm::vec4 Minion::getColor() const {
    return color;
}

float Minion::getMaxLife() const {
    return maxLife;
}

float Minion::getLife() const {
    return life;
}

float Minion::getDecay() const {
    return decay;
}

void Minion::setColor(glm::vec4 c) {
    color = c;
}

void Minion::setMaxLife(float l) {
    maxLife = l;
}

void Minion::setLife(float l) {
    life = l;
}

void Minion::setDecay(float d) {
    decay = d;
}
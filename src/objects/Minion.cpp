#define GLM_FORCE_RADIANS

#include "Minion.h"
#include "../helpers/MathHelpers.h"
#include "../constants/SimulationConstants.h"
#include "../rendering/Renderer.h"

#include <algorithm>
#include <cmath>
#include <GL/glm/gtc/quaternion.hpp>
#include <GL/glm/gtx/quaternion.hpp>
#include <GL/glm/gtx/vector_angle.hpp>

Minion::Minion(std::shared_ptr<Model> m,
        glm::vec2 p,
        float angle,
        glm::vec2 v,
        glm::vec4 c,
        float s,
        float mass) :
        model(m), pos(p), angle(angle), velocity(v), color(c), scale(s), mass(mass)
{

}

Minion::~Minion() {
}

void Minion::draw() const {
    Renderer &r = Renderer::getInstance();
    
    r.pushMatrix(Renderer::Matrix::MODEL);
    r.transform(pos.x, pos.y, 0.f, 
            angle, scale, scale);
    model->draw();
    r.popMatrix(Renderer::Matrix::MODEL);
}

void Minion::update(float deltaT) {

}

glm::vec4 Minion::getColor() const {
    return color;
}

float Minion::getMass() const {
    return mass;
}

std::shared_ptr<Model> Minion::getModel() const {
    return model;
}

float Minion::getAngle() const {
    return angle;
}

float Minion::getScale() const {
    return scale;
}

glm::vec2 Minion::getVelocity() const {
    return velocity;
}

glm::vec2 Minion::getPos() const {
    return pos;
}

void Minion::setColor(glm::vec4 c) {
    color = c;
}

void Minion::setMass(float m) {
    mass = m;
}

void Minion::setModel(std::shared_ptr<Model> m) {
    model = m;
}

void Minion::setAngle(float a) {
    angle = a;
}

void Minion::setPos(glm::vec2 p) {
    pos = p;
}

void Minion::setScale(float s) {
    scale = s;
}

void Minion::setVelocity(glm::vec2 v) {
    float vLen = glm::length2(v);
    if(vLen > std::pow(SimConst::MINION_MAX_VELOCITY, 2)){
        velocity = SimConst::MINION_MAX_VELOCITY * glm::normalize(v);
    } else {
        velocity = v;
    }
}
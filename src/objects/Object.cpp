#define GLM_FORCE_RADIANS

#include <GL/glm/gtx/rotate_vector.hpp>
#include <GL/glm/gtx/norm.hpp>
#include "Object.h"

void Object::update(float deltaT) {
    const float FRICTION_COEFF_TRANSL = 1e-7f;
    const float FRICTION_COEFF_ROT = 1e-5f;

    setAngleVel(angleVel + deltaT * angleAcc - FRICTION_COEFF_ROT * std::fabs(angleVel) * angleVel);
    setAngle(angle + deltaT * angleVel);
    setAngleAcc(0.f);

    glm::vec2 orient = glm::rotate(glm::vec2(0.f, 1.f), angle);
    setVelocity(velocity + deltaT * acceleration - FRICTION_COEFF_TRANSL * glm::length2(velocity) * velocity);
    setPos(pos + deltaT * velocity);
    setAcceleration(glm::vec2(0.f, 0.f));
}

const glm::vec2 &Object::getPos() const {
    return pos;
}

void Object::setPos(const glm::vec2 &pos) {
    Object::pos = pos;
}

const glm::vec2 &Object::getVelocity() const {
    return velocity;
}

void Object::setVelocity(const glm::vec2 &velocity) {
    Object::velocity = velocity;
}

const glm::vec2 &Object::getAcceleration() const {
    return acceleration;
}

void Object::setAcceleration(const glm::vec2 &acceleration) {
    Object::acceleration = acceleration;
}

float Object::getAngle() const {
    return angle;
}

void Object::setAngle(float angle) {
    Object::angle = angle;
    Object::front = glm::rotate(glm::vec2(0.f, 1.f), angle);
}

float Object::getAngleVel() const {
    return angleVel;
}

void Object::setAngleVel(float angleVel) {
    Object::angleVel = angleVel;
}

float Object::getAngleAcc() const {
    return angleAcc;
}

void Object::setAngleAcc(float angleAcc) {
    Object::angleAcc = angleAcc;
}

float Object::getRMass() const {
    return rMass;
}

void Object::setRMass(float rMass) {
    Object::rMass = rMass;
}

float Object::getRInertia() const {
    return rInertia;
}

glm::vec2 Object::getFront() const {
    return front;
}

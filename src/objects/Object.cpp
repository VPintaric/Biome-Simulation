#define GLM_FORCE_RADIANS

#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/norm.hpp"
#include "objects/Object.h"

void Object::update(float deltaT) {
    const float FRICTION_COEFF_TRANSL = 1e-3f;
    const float FRICTION_COEFF_ROT = 1e-2f;

    // Apply air resistance
    setAngleAcc(getAngleAcc() - FRICTION_COEFF_ROT * getRInertia() * std::fabs(getAngleVel()) * getAngleVel());
    setAcceleration(getAcceleration() - FRICTION_COEFF_TRANSL * getRMass() * glm::length2(getVelocity()) * getVelocity());

    // Numerical integration for angle velocity and position using Simpson's rule
    float ak1, ak2, ak3;
    ak1 = getAngleVel();
    ak2 = getAngleVel() + 0.5f * deltaT * getAngleAcc();
    ak3 = getAngleVel() + deltaT * getAngleAcc();
    setAngleVel(ak3);
    setAngle(getAngle() + deltaT * (ak1 + 4.f * ak2 + ak3) / 6.f);
    setAngleAcc(0.f);

    // Numerical integration for translational velocity and position using Simpson's rule
    glm::vec2 vk1, vk2, vk3;
    vk1 = getVelocity();
    vk2 = getVelocity() + 0.5f * deltaT * getAcceleration();
    vk3 = getVelocity() + deltaT * getAcceleration();
    setVelocity(vk3);
    setPos(getPos() + deltaT * (vk1 + 4.f * vk2 + vk3) / 6.f);
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

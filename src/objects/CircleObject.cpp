#define GLM_FORCE_RADIANS

#include <GL/glm/gtx/rotate_vector.hpp>
#include "CircleObject.h"

void CircleObject::update(float deltaT) {
    setAngle(angle + deltaT * angleAcc);

    glm::vec2 orient = glm::rotate(glm::vec2(0.f, 1.f), glm::radians(angle));
    setVelocity(velocity + deltaT * acceleration);
    setPos(pos + deltaT * velocity);
}

void CircleObject::setRadius(float r) {
    CircleObject::radius = r;
}

void CircleObject::setPos(glm::vec2 p) {
    CircleObject::pos = p;
}

float CircleObject::getRadius() const {
    return radius;
}

glm::vec2 CircleObject::getPos() const {
    return pos;
}

glm::vec2 CircleObject::getVelocity() const {
    return velocity;
}

void CircleObject::setVelocity(const glm::vec2 &velocity) {
    CircleObject::velocity = velocity;
}

float CircleObject::getAngle() const {
    return angle;
}

void CircleObject::setAngle(float angle) {
    CircleObject::angle = angle;
}

float CircleObject::getAngleAcc() const {
    return angleAcc;
}

void CircleObject::setAngleAcc(float angleAcc) {
    CircleObject::angleAcc = angleAcc;
}

float CircleObject::getRMass() const {
    return rMass;
}

void CircleObject::setRMass(float mass) {
    CircleObject::rMass = mass;
}

glm::vec2 CircleObject::getAcceleration() const {
    return acceleration;
}

void CircleObject::setAcceleration(const glm::vec2 &acceleration) {
    CircleObject::acceleration = acceleration;
}

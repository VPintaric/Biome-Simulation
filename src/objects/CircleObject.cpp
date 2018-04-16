#define GLM_FORCE_RADIANS

#include "objects/CircleObject.h"

void CircleObject::setRadius(float r) {
    CircleObject::radius = r;
}

float CircleObject::getRadius() const {
    return radius;
}

void CircleObject::setRMass(float rMass) {
    Object::setRMass(rMass);
    rInertia = (2 * Object::getRMass()) / (radius * radius);
}

Object::Type CircleObject::getType() const {
    return Type::CIRCLE;
}

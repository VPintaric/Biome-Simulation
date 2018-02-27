#include "objects/HollowCircleObject.h"

Object::Type HollowCircleObject::getType() const {
    return Type::HOLLOW_CIRCLE;
}

float HollowCircleObject::getR1() const {
    return r1;
}

float HollowCircleObject::getR2() const {
    return r2;
}

void HollowCircleObject::setR2(float r2) {
    HollowCircleObject::r2 = r2;
    r1 = 0.95f * r2;
}

void HollowCircleObject::setRMass(float rMass) {
    Object::setRMass(rMass);
    rInertia = (2.f * getRMass()) / (r1 * r1 + r2 * r2);
}

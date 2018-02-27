#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

#ifndef BIOME_SIMULATION_OBJECT_H
#define BIOME_SIMULATION_OBJECT_H

class Object {
public:
    enum Type{
        CIRCLE, HOLLOW_CIRCLE
    };

    virtual void update(float deltaT);

    virtual const glm::vec2 &getPos() const;

    virtual void setPos(const glm::vec2 &pos);

    virtual const glm::vec2 &getVelocity() const;

    virtual void setVelocity(const glm::vec2 &velocity);

    virtual const glm::vec2 &getAcceleration() const;

    virtual void setAcceleration(const glm::vec2 &acceleration);

    virtual float getAngle() const;

    virtual void setAngle(float angle);

    virtual float getAngleVel() const;

    virtual void setAngleVel(float angleVel);

    virtual float getAngleAcc() const;

    virtual void setAngleAcc(float angleAcc);

    virtual float getRMass() const;

    virtual void setRMass(float rMass);

    virtual float getRInertia() const;

    virtual Type getType() const = 0;

    virtual glm::vec2 getFront() const;

protected:
    glm::vec2 pos, velocity, acceleration, front;
    float angle, angleVel, angleAcc, rMass, rInertia;
};


#endif //BIOME_SIMULATION_OBJECT_H

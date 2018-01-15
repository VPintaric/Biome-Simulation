#ifndef BIOME_SIMULATION_CIRCLEOBJECT_H
#define BIOME_SIMULATION_CIRCLEOBJECT_H


#include <GL/glm/glm.hpp>

class CircleObject {
public:
    virtual void update(float deltaT);

    virtual void setRadius(float r);

    virtual void setPos(glm::vec2 p);

    virtual float getRadius() const;

    virtual glm::vec2 getPos() const;

    virtual glm::vec2 getVelocity() const;

    virtual void setVelocity(const glm::vec2 &velocity);

    virtual float getAngle() const;

    virtual void setAngle(float angle);

    virtual float getAngleAcc() const;

    virtual void setAngleAcc(float angleAcc);

    virtual float getRMass() const;

    virtual void setRMass(float mass);

    glm::vec2 getAcceleration() const;

    void setAcceleration(const glm::vec2 &acceleration);

private:
    glm::vec2 pos, velocity, acceleration;
    float radius, angle, angleAcc, rMass;
};


#endif //BIOME_SIMULATION_CIRCLEOBJECT_H

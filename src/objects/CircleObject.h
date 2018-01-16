#ifndef BIOME_SIMULATION_CIRCLEOBJECT_H
#define BIOME_SIMULATION_CIRCLEOBJECT_H

#define GLM_FORCE_RADIANS

#include <GL/glm/glm.hpp>
#include "Object.h"

class CircleObject : public Object{
public:
    void setRadius(float r);
    float getRadius() const;

    void setRMass(float rMass) override;

    Type getType() const override;

private:
    float radius;
};


#endif //BIOME_SIMULATION_CIRCLEOBJECT_H

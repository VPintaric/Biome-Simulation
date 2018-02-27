#ifndef BIOME_SIMULATION_HOLLOWCIRCLEOBJECT_H
#define BIOME_SIMULATION_HOLLOWCIRCLEOBJECT_H


#include "Object.h"

class HollowCircleObject : public Object {
public:
    Type getType() const override;

    float getR1() const;

    float getR2() const;

    void setR2(float r2);

    void setRMass(float rMass) override;


private:
    float r1, r2;
};


#endif //BIOME_SIMULATION_HOLLOWCIRCLEOBJECT_H

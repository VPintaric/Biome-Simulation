#ifndef BIOME_SIMULATION_MINIONSENSES_H
#define BIOME_SIMULATION_MINIONSENSES_H

#include <objects/CircleObject.h>
#include <vector>
#include <minion/Minion.h>

class Minion;

class MinionSenses : public CircleObject {
public:
    MinionSenses();
    virtual ~MinionSenses();

    virtual void setMaxSenseDistance(float dist) = 0;
    virtual float getMaxSenseDistance() = 0;
    virtual int getDataSize() = 0;
    virtual std::vector<float> gatherData(float deltaT) = 0;
    virtual void draw() = 0;
};


#endif //BIOME_SIMULATION_MINIONSENSES_H

#ifndef BIOME_SIMULATION_MINIONOBJECTCROSSOVER_H
#define BIOME_SIMULATION_MINIONOBJECTCROSSOVER_H

#include <config/JSONConfigurable.h>
#include <minion/object/MinionObject.h>

class MinionObjectCrossover : public JSONConfigurable{
public:
    virtual std::shared_ptr<MinionObject> crossover(std::shared_ptr<MinionObject> p1,
                                                    std::shared_ptr<MinionObject> p2,
                                                    float f1, float f2) = 0;
};

#endif //BIOME_SIMULATION_MINIONOBJECTCROSSOVER_H

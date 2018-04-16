#ifndef BIOME_SIMULATION_FITNESS_H
#define BIOME_SIMULATION_FITNESS_H


#include <config/JSONConfigurable.h>
#include <minion/Minion.h>

class Fitness : public JSONConfigurable {
public:
    virtual void calculateFitness(std::shared_ptr<Minion> m) = 0;
};


#endif //BIOME_SIMULATION_FITNESS_H

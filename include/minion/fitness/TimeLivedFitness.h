#ifndef BIOME_SIMULATION_TIMELIVEDFITNESS_H
#define BIOME_SIMULATION_TIMELIVEDFITNESS_H


#include "Fitness.h"

class TimeLivedFitness : public Fitness {
public:
    void configureFromJSON(rjs::Value &root) override;

    void calculateFitness(std::shared_ptr<Minion> m) override;

};


#endif //BIOME_SIMULATION_TIMELIVEDFITNESS_H

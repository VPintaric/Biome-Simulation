#ifndef BIOME_SIMULATION_FITNESSWEIGHTEDAVERAGESENSESCROSSOVER_H
#define BIOME_SIMULATION_FITNESSWEIGHTEDAVERAGESENSESCROSSOVER_H


#include <minion/crossover_operators/senses/MinionSensesCrossover.h>

class FitnessWeightedAverageSensesCrossover : public MinionSensesCrossover {
public:
    void configureFromJSON(rjs::Value &root) override;

    std::shared_ptr<MinionSenses>
    crossover(std::shared_ptr<MinionSenses> p1, std::shared_ptr<MinionSenses> p2, float f1, float f2) override;

};


#endif //BIOME_SIMULATION_FITNESSWEIGHTEDAVERAGESENSESCROSSOVER_H

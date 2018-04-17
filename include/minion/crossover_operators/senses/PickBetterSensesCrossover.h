#ifndef BIOME_SIMULATION_PICKBETTERSENSESCROSSOVER_H
#define BIOME_SIMULATION_PICKBETTERSENSESCROSSOVER_H


#include <minion/crossover_operators/senses/MinionSensesCrossover.h>

class PickBetterSensesCrossover : public MinionSensesCrossover {
public:
    void configureFromJSON(rjs::Value &root) override;

    std::shared_ptr<MinionSenses>
    crossover(std::shared_ptr<MinionSenses> p1, std::shared_ptr<MinionSenses> p2, float f1, float f2) override;
};


#endif //BIOME_SIMULATION_PICKBETTERSENSESCROSSOVER_H

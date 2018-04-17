#ifndef BIOME_SIMULATION_PICKBETTEROBJECTCROSSOVER_H
#define BIOME_SIMULATION_PICKBETTEROBJECTCROSSOVER_H


#include <minion/crossover_operators/object/MinionObjectCrossover.h>

class PickBetterObjectCrossover : public MinionObjectCrossover {
public:
    void configureFromJSON(rjs::Value &root) override;

    std::shared_ptr<MinionObject>
    crossover(std::shared_ptr<MinionObject> p1, std::shared_ptr<MinionObject> p2, float f1, float f2) override;

};


#endif //BIOME_SIMULATION_PICKBETTEROBJECTCROSSOVER_H

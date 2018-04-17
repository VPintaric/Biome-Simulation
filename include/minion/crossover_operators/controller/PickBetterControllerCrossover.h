#ifndef BIOME_SIMULATION_PICKBETTERCONTROLLERCROSSOVER_H
#define BIOME_SIMULATION_PICKBETTERCONTROLLERCROSSOVER_H


#include <minion/crossover_operators/controller/MinionControllerCrossover.h>

class PickBetterControllerCrossover : public MinionControllerCrossover {
public:
    void configureFromJSON(rjs::Value &root) override;

    std::shared_ptr<MinionController>
    crossover(std::shared_ptr<MinionController> p1, std::shared_ptr<MinionController> p2, float f1, float f2) override;

};


#endif //BIOME_SIMULATION_PICKBETTERCONTROLLERCROSSOVER_H

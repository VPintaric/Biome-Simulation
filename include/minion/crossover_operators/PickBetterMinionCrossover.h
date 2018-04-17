#ifndef BIOME_SIMULATION_PICKBETTERCROSSOVER_H
#define BIOME_SIMULATION_PICKBETTERCROSSOVER_H


#include "MinionCrossover.h"

class PickBetterMinionCrossover : public MinionCrossover {
public:
    PickBetterMinionCrossover();
    ~PickBetterMinionCrossover();

    void configureFromJSON(rjs::Value &root) override;

    std::shared_ptr<Minion> crossover(std::shared_ptr<Minion> m1, std::shared_ptr<Minion> m2) override;

};


#endif //BIOME_SIMULATION_PICKBETTERCROSSOVER_H

#ifndef BIOME_SIMULATION_PICKRANDOMMINIONCROSSOVER_H
#define BIOME_SIMULATION_PICKRANDOMMINIONCROSSOVER_H


#include <minion/crossover_operators/MinionCrossover.h>

class PickRandomMinionCrossover : public MinionCrossover {
public:
    void configureFromJSON(rjs::Value &root) override;

    std::shared_ptr<Minion> crossover(std::shared_ptr<Minion> m1, std::shared_ptr<Minion> m2) override;

};


#endif //BIOME_SIMULATION_PICKRANDOMMINIONCROSSOVER_H

#ifndef BIOME_SIMULATION_MINIONCROSSOVER_H
#define BIOME_SIMULATION_MINIONCROSSOVER_H

#include <config/JSONConfigurable.h>
#include <minion/factories/MinionGenerator.h>

class MinionCrossover : public JSONConfigurable {
protected:
    std::shared_ptr<MinionGenerator> minionGenerator;

public:

    virtual std::shared_ptr<Minion> crossover(std::shared_ptr<Minion> m1, std::shared_ptr<Minion> m2) = 0;
};

#endif //BIOME_SIMULATION_MINIONCROSSOVER_H

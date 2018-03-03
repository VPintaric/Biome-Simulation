#ifndef BIOME_SIMULATION_MINIONGENERATOR_H
#define BIOME_SIMULATION_MINIONGENERATOR_H


#include <minion/Minion.h>

class MinionGenerator {
public:
    MinionGenerator();
    virtual ~MinionGenerator();

    virtual std::shared_ptr<Minion> generateMinion() = 0;
};


#endif //BIOME_SIMULATION_MINIONGENERATOR_H

#ifndef BIOME_SIMULATION_MINIONGENERATOR_H
#define BIOME_SIMULATION_MINIONGENERATOR_H


#include <minion/Minion.h>

class MinionGenerator {
public:
    MinionGenerator();
    virtual ~MinionGenerator();

    virtual std::shared_ptr<Minion> generateMinion() = 0;
    virtual std::shared_ptr<Minion> generateChild(std::shared_ptr<Minion> first, std::shared_ptr<Minion> second) = 0;
};


#endif //BIOME_SIMULATION_MINIONGENERATOR_H

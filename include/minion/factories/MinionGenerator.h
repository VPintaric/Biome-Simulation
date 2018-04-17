#ifndef BIOME_SIMULATION_MINIONGENERATOR_H
#define BIOME_SIMULATION_MINIONGENERATOR_H


#include <minion/Minion.h>
#include <config/JSONConfigurable.h>

class MinionGenerator : public JSONConfigurable {
public:
    virtual std::shared_ptr<Minion> generateRandomMinion() = 0;
    virtual std::shared_ptr<Minion> generateMinion() = 0;
};


#endif //BIOME_SIMULATION_MINIONGENERATOR_H

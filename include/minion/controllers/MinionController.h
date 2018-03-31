#ifndef BIOME_SIMULATION_MINIONCONTROLLER_H
#define BIOME_SIMULATION_MINIONCONTROLLER_H

#include "minion/object/MinionObject.h"
#include "minion/senses/MinionSenses.h"

class MinionController {
public:
    MinionController();
    virtual ~MinionController();

    virtual std::vector<float> controlMinion(std::vector<float> senseData) = 0;
};


#endif //BIOME_SIMULATION_MINIONCONTROLLER_H

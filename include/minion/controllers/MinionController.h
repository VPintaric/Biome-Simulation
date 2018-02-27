#ifndef BIOME_SIMULATION_MINIONCONTROLLER_H
#define BIOME_SIMULATION_MINIONCONTROLLER_H

#include "minion/object/MinionObject.h"
#include "minion/senses/MinionSenses.h"

class MinionController {
public:
    MinionController();
    virtual ~MinionController();

    virtual void controlMinion(const std::shared_ptr<MinionObject> &m, const std::shared_ptr<MinionSenses> &senses) = 0;
private:

};


#endif //BIOME_SIMULATION_MINIONCONTROLLER_H

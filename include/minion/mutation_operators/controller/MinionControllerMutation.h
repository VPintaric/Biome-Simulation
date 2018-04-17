#ifndef BIOME_SIMULATION_MINIONCONTROLLERMUTATION_H
#define BIOME_SIMULATION_MINIONCONTROLLERMUTATION_H

#include <config/JSONConfigurable.h>
#include <minion/controllers/MinionController.h>

class MinionControllerMutation : public JSONConfigurable {
public:
    virtual void mutate(std::shared_ptr<MinionController> i) = 0;
};

#endif //BIOME_SIMULATION_MINIONCONTROLLERMUTATION_H

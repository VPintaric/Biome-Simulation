#ifndef BIOME_SIMULATION_MINIONOBJECTMUTATION_H
#define BIOME_SIMULATION_MINIONOBJECTMUTATION_H

#include <config/JSONConfigurable.h>
#include <minion/object/MinionObject.h>

class MinionObjectMutation : public JSONConfigurable {
public:
    virtual void mutate(std::shared_ptr<MinionObject> i) = 0;
};

#endif //BIOME_SIMULATION_MINIONOBJECTMUTATION_H

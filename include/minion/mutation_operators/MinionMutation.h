#ifndef BIOME_SIMULATION_MINIONMUTATION_H
#define BIOME_SIMULATION_MINIONMUTATION_H

#include <config/JSONConfigurable.h>
#include <minion/Minion.h>

class MinionMutation : public JSONConfigurable {
public:
    virtual void mutate(std::shared_ptr<Minion> m) = 0;
};

#endif //BIOME_SIMULATION_MINIONMUTATION_H

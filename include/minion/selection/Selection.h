#ifndef BIOME_SIMULATION_SELECTION_H
#define BIOME_SIMULATION_SELECTION_H


#include <minion/Minion.h>
#include <minion/factories/MinionGenerator.h>

class Selection : public JSONConfigurable {
public:
    virtual std::pair<std::shared_ptr<Minion>, std::shared_ptr<Minion>>
        selectParents(std::vector<std::shared_ptr<Minion>> &minions) = 0;
};

#endif //BIOME_SIMULATION_SELECTION_H

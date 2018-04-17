#ifndef BIOME_SIMULATION_MINIONSENSESMUTATION_H
#define BIOME_SIMULATION_MINIONSENSESMUTATION_H

#include <config/JSONConfigurable.h>
#include <minion/senses/MinionSenses.h>

class MinionSensesMutation : public JSONConfigurable {
public:
    virtual void mutate(std::shared_ptr<MinionSenses> i) = 0;
};

#endif //BIOME_SIMULATION_MINIONSENSESMUTATION_H

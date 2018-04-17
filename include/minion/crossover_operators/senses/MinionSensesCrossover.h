#ifndef BIOME_SIMULATION_MINIONSENSESCROSSOVER_H
#define BIOME_SIMULATION_MINIONSENSESCROSSOVER_H

#include <config/JSONConfigurable.h>
#include <minion/senses/MinionSenses.h>

class MinionSensesCrossover : public JSONConfigurable {
public:
    virtual std::shared_ptr<MinionSenses> crossover(std::shared_ptr<MinionSenses> p1,
                                                    std::shared_ptr<MinionSenses> p2,
                                                    float f1, float f2) = 0;
};

#endif //BIOME_SIMULATION_MINIONSENSESCROSSOVER_H

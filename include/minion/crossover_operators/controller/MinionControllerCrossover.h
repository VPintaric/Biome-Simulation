#ifndef BIOME_SIMULATION_MINIONCONTROLLERCROSSOVER_H
#define BIOME_SIMULATION_MINIONCONTROLLERCROSSOVER_H

#include <config/JSONConfigurable.h>
#include <minion/controllers/MinionController.h>

class MinionControllerCrossover : public JSONConfigurable {
public:
    virtual std::shared_ptr<MinionController> crossover(std::shared_ptr<MinionController> p1,
                                                        std::shared_ptr<MinionController> p2,
                                                        float f1, float f2) = 0;
};

#endif //BIOME_SIMULATION_MINIONCONTROLLERCROSSOVER_H

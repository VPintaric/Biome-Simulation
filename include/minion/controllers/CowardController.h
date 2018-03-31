#ifndef BIOME_SIMULATION_COWARDCONTROLLER_H
#define BIOME_SIMULATION_COWARDCONTROLLER_H


#include "MinionController.h"

class CowardController : public MinionController{
public:
    CowardController();
    ~CowardController() override;

    std::vector<float> controlMinion(std::vector<float> senseData) override;
};


#endif //BIOME_SIMULATION_COWARDCONTROLLER_H

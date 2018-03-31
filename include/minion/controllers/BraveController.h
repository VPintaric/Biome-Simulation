#ifndef BIOME_SIMULATION_BRAVECONTROLLER_H
#define BIOME_SIMULATION_BRAVECONTROLLER_H


#include "MinionController.h"

class BraveController : public MinionController{
public:
    BraveController();
    ~BraveController() override;

    std::vector<float> controlMinion(std::vector<float> senseData) override;
};


#endif //BIOME_SIMULATION_BRAVECONTROLLER_H

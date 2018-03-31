#ifndef BIOME_SIMULATION_NULLCONTROLLER_H
#define BIOME_SIMULATION_NULLCONTROLLER_H


#include "MinionController.h"

class NullController : public MinionController {
public:
    NullController();
    ~NullController() override;

    std::vector<float> controlMinion(std::vector<float> senseData) override;
};


#endif //BIOME_SIMULATION_NULLCONTROLLER_H

#ifndef BIOME_SIMULATION_RANDOMCONTROLLER_H
#define BIOME_SIMULATION_RANDOMCONTROLLER_H


#include "MinionController.h"

class RandomController : public MinionController{
public:
    RandomController();
    ~RandomController() override;

    std::vector<float> controlMinion(std::vector<float> senseData) override;

private:

};


#endif //BIOME_SIMULATION_RANDOMCONTROLLER_H

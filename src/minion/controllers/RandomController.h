#ifndef BIOME_SIMULATION_RANDOMCONTROLLER_H
#define BIOME_SIMULATION_RANDOMCONTROLLER_H


#include "MinionController.h"

class RandomController : public MinionController{
public:
    RandomController();
    ~RandomController() override;

    void controlMinion(const std::shared_ptr<MinionObject> &m, const std::shared_ptr<MinionSenses> &senses) override;

private:

};


#endif //BIOME_SIMULATION_RANDOMCONTROLLER_H

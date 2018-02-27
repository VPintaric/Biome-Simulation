#ifndef BIOME_SIMULATION_COWARDCONTROLLER_H
#define BIOME_SIMULATION_COWARDCONTROLLER_H


#include "MinionController.h"

class CowardController : public MinionController{
public:
    CowardController();
    ~CowardController() override;
    void controlMinion(const std::shared_ptr<MinionObject> &m, const std::shared_ptr<MinionSenses> &senses) override;
};


#endif //BIOME_SIMULATION_COWARDCONTROLLER_H

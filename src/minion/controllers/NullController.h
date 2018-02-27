#ifndef BIOME_SIMULATION_NULLCONTROLLER_H
#define BIOME_SIMULATION_NULLCONTROLLER_H


#include "MinionController.h"

class NullController : public MinionController {
public:
    NullController();
    ~NullController() override;

    void controlMinion(const std::shared_ptr<MinionObject> &m, const std::shared_ptr<MinionSenses> &senses) override;
};


#endif //BIOME_SIMULATION_NULLCONTROLLER_H

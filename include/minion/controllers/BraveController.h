#ifndef BIOME_SIMULATION_BRAVECONTROLLER_H
#define BIOME_SIMULATION_BRAVECONTROLLER_H


#include "MinionController.h"

class BraveController : public MinionController{
public:
    BraveController();
    ~BraveController() override;
    void controlMinion(const std::shared_ptr<MinionObject> &m, const std::shared_ptr<MinionSenses> &senses) override;
private:
};


#endif //BIOME_SIMULATION_BRAVECONTROLLER_H

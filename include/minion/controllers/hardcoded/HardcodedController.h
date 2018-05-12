#ifndef BIOME_SIMULATION_HARDCODEDCONTROLLER_H
#define BIOME_SIMULATION_HARDCODEDCONTROLLER_H


#include <minion/controllers/MinionController.h>

class HardcodedController : public MinionController {
private:
    std::weak_ptr<Minion> m;

    std::shared_ptr<Minion> lockMinion();

public:
    std::shared_ptr<MinionController> copy() override;

    std::vector<float> controlMinion(std::vector<float> senseData) override;

    void setMinion(std::shared_ptr<Minion> m) override;
};


#endif //BIOME_SIMULATION_HARDCODEDCONTROLLER_H

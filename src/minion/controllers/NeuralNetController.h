#ifndef BIOME_SIMULATION_NEURALNETCONTROLLER_H
#define BIOME_SIMULATION_NEURALNETCONTROLLER_H


#include "MinionController.h"

class NeuralNetController : public MinionController {
public:
    NeuralNetController();
    ~NeuralNetController() override;
    void controlMinion(const std::shared_ptr<MinionObject> &m, const std::shared_ptr<MinionSenses> &senses) override;
};


#endif //BIOME_SIMULATION_NEURALNETCONTROLLER_H

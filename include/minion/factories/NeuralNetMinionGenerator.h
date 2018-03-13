#ifndef BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H
#define BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H


#include "MinionGenerator.h"

class NeuralNetMinionGenerator : public MinionGenerator {
public:
    NeuralNetMinionGenerator();
    virtual ~NeuralNetMinionGenerator();

    std::shared_ptr<Minion> generateMinion() override;
private:

};


#endif //BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H

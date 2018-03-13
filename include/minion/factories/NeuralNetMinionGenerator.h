#ifndef BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H
#define BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H


#include "MinionGenerator.h"

class NeuralNetMinionGenerator : public MinionGenerator {
private:

public:
    std::shared_ptr<Minion> generateMinion() override;

};


#endif //BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H

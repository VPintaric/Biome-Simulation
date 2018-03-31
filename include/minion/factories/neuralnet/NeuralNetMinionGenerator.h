#ifndef BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H
#define BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H


#include "minion/factories/MinionGenerator.h"

class NeuralNetMinionGenerator : public MinionGenerator {
public:
    NeuralNetMinionGenerator();
    ~NeuralNetMinionGenerator() override;

    std::shared_ptr<Minion> generateMinion() override;

    std::shared_ptr<Minion> generateChild(std::shared_ptr<Minion> first, std::shared_ptr<Minion> second) override;

private:
    std::shared_ptr<Minion> crossover(std::shared_ptr<Minion> first, std::shared_ptr<Minion> second);
    void mutate(std::shared_ptr<Minion> m);
};


#endif //BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H

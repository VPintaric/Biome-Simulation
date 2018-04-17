#ifndef BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H
#define BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H


#include "minion/factories/MinionGenerator.h"
#include <random>
#include <minion/factories/neuralnet/mutation/NeuralNetMutation.h>
#include <minion/factories/neuralnet/crossover/NeuralNetCrossover.h>

class NeuralNetMinionGenerator : public MinionGenerator {
public:
    explicit NeuralNetMinionGenerator(std::vector<int> nnHiddenLayers = {40, 30});
    ~NeuralNetMinionGenerator() override;

    std::shared_ptr<Minion> generateRandomMinion() override;

    std::shared_ptr<Minion> generateMinion() override;

    void configureFromJSON(rjs::Value &root) override;

private:
    std::vector<int> nnHiddenLayers;
};


#endif //BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H

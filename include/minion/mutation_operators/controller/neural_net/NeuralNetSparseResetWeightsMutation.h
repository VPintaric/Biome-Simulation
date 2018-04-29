#ifndef BIOME_SIMULATION_NEURALNETSPARSERESETWEIGHTSMUTATION_H
#define BIOME_SIMULATION_NEURALNETSPARSERESETWEIGHTSMUTATION_H


#include <random>
#include "NeuralNetControllerMutation.h"

class NeuralNetSparseResetWeightsMutation : public NeuralNetControllerMutation {
private:
    float weightProbability, biasProbability, variance, mean;
    std::normal_distribution<float> distr;

    void nnMut(std::shared_ptr<NeuralNet> nn);

public:
    NeuralNetSparseResetWeightsMutation();

    void neuralNetMutation(std::shared_ptr<NeuralNetController> i) override;

    void configureFromJSON(rjs::Value &root) override;
};


#endif //BIOME_SIMULATION_NEURALNETSPARSERESETWEIGHTSMUTATION_H

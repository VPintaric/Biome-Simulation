#ifndef BIOME_SIMULATION_NEURALNETGAUSSNOISEWEIGHTS_H
#define BIOME_SIMULATION_NEURALNETGAUSSNOISEWEIGHTS_H


#include <minion/mutation_operators/controller/neural_net/NeuralNetControllerMutation.h>
#include <random>

class NeuralNetGaussNoiseWeightsMutation : public NeuralNetControllerMutation {
private:
    float mean, variance;
    std::normal_distribution<float> distr;

public:
    NeuralNetGaussNoiseWeightsMutation();

    void neuralNetMutation(std::shared_ptr<NeuralNetController> i) override;
    void configureFromJSON(rjs::Value& root) override;
};


#endif //BIOME_SIMULATION_NEURALNETGAUSSNOISEWEIGHTS_H

#ifndef BIOME_SIMULATION_NEURALNETCROSSOVER_H
#define BIOME_SIMULATION_NEURALNETCROSSOVER_H


#include <minion/controllers/neural_net/NeuralNet.h>
#include <config/JSONConfigurable.h>

class NeuralNetCrossover : public JSONConfigurable {
public:
    NeuralNetCrossover();
    virtual ~NeuralNetCrossover();

    virtual void crossover(std::shared_ptr<NeuralNet> p1, std::shared_ptr<NeuralNet> p2, std::shared_ptr<NeuralNet> c) = 0;
};


#endif //BIOME_SIMULATION_NEURALNETCROSSOVER_H

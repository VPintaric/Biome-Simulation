#ifndef BIOME_SIMULATION_NEURALNETMUTATION_H
#define BIOME_SIMULATION_NEURALNETMUTATION_H


#include <config/JSONConfigurable.h>
#include <minion/controllers/neural_net/NeuralNet.h>

class NeuralNetMutation : public JSONConfigurable {
public:
    NeuralNetMutation();
    virtual ~NeuralNetMutation();

    virtual void mutate(std::shared_ptr<NeuralNet> nn) = 0;
};


#endif //BIOME_SIMULATION_NEURALNETMUTATION_H

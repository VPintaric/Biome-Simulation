#ifndef BIOME_SIMULATION_NEURALNETCONTROLLERMUTATION_H
#define BIOME_SIMULATION_NEURALNETCONTROLLERMUTATION_H


#include <minion/mutation_operators/controller/MinionControllerMutation.h>
#include <minion/controllers/neural_net/NeuralNetController.h>

class NeuralNetControllerMutation : public MinionControllerMutation {
public:
    void mutate(std::shared_ptr<MinionController> i) override;

    virtual void neuralNetMutation(std::shared_ptr<NeuralNetController> i) = 0;
};


#endif //BIOME_SIMULATION_NEURALNETCONTROLLERMUTATION_H

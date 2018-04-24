#ifndef BIOME_SIMULATION_NEURALNETCONTROLLERCROSSOVER_H
#define BIOME_SIMULATION_NEURALNETCONTROLLERCROSSOVER_H


#include <minion/crossover_operators/controller/MinionControllerCrossover.h>
#include <minion/controllers/neural_net/NeuralNetController.h>

class NeuralNetControllerCrossover : public MinionControllerCrossover {
public:
    std::shared_ptr<MinionController> crossover(std::shared_ptr<MinionController> p1,
                                                std::shared_ptr<MinionController> p2,
                                                float f1, float f2) override;

    virtual std::shared_ptr<NeuralNetController> neuralNetCrossover(std::shared_ptr<NeuralNetController> p1,
                                                                    std::shared_ptr<NeuralNetController> p2,
                                                                    float f1, float f2) = 0;
};


#endif //BIOME_SIMULATION_NEURALNETCONTROLLERCROSSOVER_H

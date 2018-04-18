#ifndef BIOME_SIMULATION_NEURALNETSINGLELAYERCROSSOVER_H
#define BIOME_SIMULATION_NEURALNETSINGLELAYERCROSSOVER_H


#include <minion/crossover_operators/controller/neural_net/NeuralNetControllerCrossover.h>

class NeuralNetSingleLayerCrossover : public NeuralNetControllerCrossover {
public:
    std::shared_ptr<NeuralNetController>
    neuralNetCrossover(std::shared_ptr<NeuralNetController> p1, std::shared_ptr<NeuralNetController> p2, float f1,
                       float f2) override;
};


#endif //BIOME_SIMULATION_NEURALNETSINGLELAYERCROSSOVER_H

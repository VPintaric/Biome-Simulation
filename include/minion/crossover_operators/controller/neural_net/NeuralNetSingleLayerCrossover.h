#ifndef BIOME_SIMULATION_NEURALNETSINGLELAYERCROSSOVER_H
#define BIOME_SIMULATION_NEURALNETSINGLELAYERCROSSOVER_H


#include <minion/crossover_operators/controller/neural_net/NeuralNetControllerCrossover.h>

class NeuralNetSingleLayerCrossover : public NeuralNetControllerCrossover {
public:
    std::shared_ptr<NeuralNetController>
    neuralNetCrossover(std::shared_ptr<NeuralNetController> p1, std::shared_ptr<NeuralNetController> p2, float f1,
                       float f2) override;

    void configureFromJSON(rjs::Value &root) override;

private:
    void nnCross(std::shared_ptr<NeuralNet> child, std::shared_ptr<NeuralNet> worse);
};


#endif //BIOME_SIMULATION_NEURALNETSINGLELAYERCROSSOVER_H

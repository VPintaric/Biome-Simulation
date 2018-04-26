#ifndef BIOME_SIMULATION_NEURALNETLAYERWEIGHTSCROSSOVER_H
#define BIOME_SIMULATION_NEURALNETLAYERWEIGHTSCROSSOVER_H


#include "NeuralNetControllerCrossover.h"

class NeuralNetLayerWeightsCrossover : public NeuralNetControllerCrossover {
public:
    void configureFromJSON(rjs::Value &root) override;

    std::shared_ptr<NeuralNetController>
    neuralNetCrossover(std::shared_ptr<NeuralNetController> p1, std::shared_ptr<NeuralNetController> p2, float f1,
                       float f2) override;

};


#endif //BIOME_SIMULATION_NEURALNETLAYERWEIGHTSCROSSOVER_H

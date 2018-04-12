#ifndef BIOME_SIMULATION_INTERLEAVELAYERSCROSSOVER_H
#define BIOME_SIMULATION_INTERLEAVELAYERSCROSSOVER_H


#include "NeuralNetCrossover.h"

class InterleaveLayersCrossover : public NeuralNetCrossover {
public:
    InterleaveLayersCrossover();
    virtual ~InterleaveLayersCrossover();

    void configureFromJSON(rjs::Value &root) override;

    void crossover(std::shared_ptr<NeuralNet> p1, std::shared_ptr<NeuralNet> p2, std::shared_ptr<NeuralNet> c) override;
};


#endif //BIOME_SIMULATION_INTERLEAVELAYERSCROSSOVER_H

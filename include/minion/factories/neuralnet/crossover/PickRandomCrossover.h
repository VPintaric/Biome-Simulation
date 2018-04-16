#ifndef BIOME_SIMULATION_PICKBETTERCROSSOVER_H
#define BIOME_SIMULATION_PICKBETTERCROSSOVER_H


#include "NeuralNetCrossover.h"

class PickRandomCrossover : public NeuralNetCrossover {
public:
    void configureFromJSON(rjs::Value &root) override;

    void crossover(std::shared_ptr<NeuralNet> p1, std::shared_ptr<NeuralNet> p2, std::shared_ptr<NeuralNet> c) override;
};


#endif //BIOME_SIMULATION_PICKBETTERCROSSOVER_H

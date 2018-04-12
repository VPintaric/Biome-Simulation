#ifndef BIOME_SIMULATION_ARITHMETICAVERAGECROSSOVER_H
#define BIOME_SIMULATION_ARITHMETICAVERAGECROSSOVER_H


#include <minion/factories/neuralnet/crossover/NeuralNetCrossover.h>

class ArithmeticAverageCrossover : public NeuralNetCrossover {
public:
    ArithmeticAverageCrossover();
    virtual ~ArithmeticAverageCrossover();

    void configureFromJSON(rjs::Value &root) override;

    void crossover(std::shared_ptr<NeuralNet> p1, std::shared_ptr<NeuralNet> p2, std::shared_ptr<NeuralNet> c) override;
};


#endif //BIOME_SIMULATION_ARITHMETICAVERAGECROSSOVER_H

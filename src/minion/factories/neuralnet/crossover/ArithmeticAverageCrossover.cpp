#include <state/Log.h>
#include "minion/factories/neuralnet/crossover/ArithmeticAverageCrossover.h"

ArithmeticAverageCrossover::ArithmeticAverageCrossover() = default;
ArithmeticAverageCrossover::~ArithmeticAverageCrossover() = default;

void ArithmeticAverageCrossover::configureFromJSON(rjs::Value &root) {
    // No parameters to configure
}

void ArithmeticAverageCrossover::crossover(std::shared_ptr<NeuralNet> p1, std::shared_ptr<NeuralNet> p2,
                                           std::shared_ptr<NeuralNet> c) {
    for(int layer = 0; layer < c->weights.size(); layer++){
        *c->weights[layer] = 0.5f * (*p1->weights[layer] + *p2->weights[layer]);
        *c->bias[layer] = 0.5f * (*p1->bias[layer] + *p2->bias[layer]);
    }
}

#include <state/State.h>
#include <helpers/RNG.h>
#include "minion/factories/neuralnet/crossover/PickRandomCrossover.h"

void PickRandomCrossover::configureFromJSON(rjs::Value &root) {
    // Nothing to configure...
}

void PickRandomCrossover::crossover(std::shared_ptr<NeuralNet> p1, std::shared_ptr<NeuralNet> p2,
                                    std::shared_ptr<NeuralNet> c) {
    std::uniform_int_distribution<> distr(0, 1);
    auto pickedParent = distr(RNG::get()) ? p1 : p2;

    for(int layer = 0; layer < c->weights.size(); layer++){
        *c->weights[layer] = *pickedParent->weights[layer];
        *c->bias[layer] = *pickedParent->bias[layer];
    }
}

#include <helpers/RNG.h>
#include "minion/crossover_operators/controller/neural_net/NeuralNetSingleLayerCrossover.h"

void NeuralNetSingleLayerCrossover::nnCross(std::shared_ptr<NeuralNet> child, std::shared_ptr<NeuralNet> worse) {
    std::uniform_int_distribution<int> distr(0, worse->weights.size() - 1);

    int layerToCopy = distr(RNG::get());

    auto childW = child->weights[layerToCopy];
    auto childB = child->bias[layerToCopy];

    auto pW = worse->weights[layerToCopy];
    auto pB = worse->bias[layerToCopy];

    *childW = *pW;
    *childB = *pB;
}

std::shared_ptr<NeuralNetController>
NeuralNetSingleLayerCrossover::neuralNetCrossover(std::shared_ptr<NeuralNetController> p1,
                                                  std::shared_ptr<NeuralNetController> p2, float f1, float f2) {
    auto better = f1 > f2 ? p1 : p2;
    auto worse = f1 > f2 ? p2 : p1;

    auto child = std::static_pointer_cast<NeuralNetController>(better->copy());

    auto caNN = child->getAccNeuralNet();
    auto waNN = worse->getAccNeuralNet();
    auto crNN = child->getRotNeuralNet();
    auto wrNN = worse->getRotNeuralNet();

    nnCross(caNN, waNN);
    nnCross(crNN, wrNN);

    return child;
}

void NeuralNetSingleLayerCrossover::configureFromJSON(rjs::Value &root) {
    // Nothing to do
}

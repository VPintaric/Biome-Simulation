#include <helpers/RNG.h>
#include "minion/crossover_operators/controller/neural_net/NeuralNetSingleLayerCrossover.h"

std::shared_ptr<NeuralNetController>
NeuralNetSingleLayerCrossover::neuralNetCrossover(std::shared_ptr<NeuralNetController> p1,
                                                  std::shared_ptr<NeuralNetController> p2, float f1, float f2) {
    auto better = f1 > f2 ? p1 : p2;
    auto worse = f1 > f2 ? p2 : p1;

    auto child = std::static_pointer_cast<NeuralNetController>(better->copy());

    auto childNN = child->getNeuralNet();
    auto pNN = worse->getNeuralNet();

    std::uniform_int_distribution<int> distr(0, pNN->weights.size() - 1);

    int layerToCopy = distr(RNG::get());

    auto childW = childNN->weights[layerToCopy];
    auto childB = childNN->bias[layerToCopy];

    auto pW = pNN->weights[layerToCopy];
    auto pB = pNN->bias[layerToCopy];

    *childW = *pW;
    *childB = *pB;

    return child;
}

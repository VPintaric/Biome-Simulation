#include <helpers/RNG.h>
#include "minion/crossover_operators/controller/neural_net/NeuralNetLayerWeightsCrossover.h"

void NeuralNetLayerWeightsCrossover::configureFromJSON(rjs::Value &root) {
    // Nothing to configure
}

void NeuralNetLayerWeightsCrossover::nnCross(std::shared_ptr<NeuralNet> child, std::shared_ptr<NeuralNet> worse) {
    for(int layer = 0; layer < child->weights.size(); layer++){
        auto cw = child->weights[layer];
        auto cb = child->bias[layer];
        auto ww = worse->weights[layer];
        auto wb = worse->bias[layer];

        std::uniform_int_distribution<int> distr(0, static_cast<int>(cw->cols()));
        int crossPoint = distr(RNG::get());

        for(int c = crossPoint; c < cw->cols(); c++){
            cb->operator()(0, c) += wb->operator()(0, c);
            cb->operator()(0, c) /= 2.f;

            for(int r = 0; r < cw->rows(); r++){
                cw->operator()(r, c) += ww->operator()(r, c);
                cw->operator()(r, c) /= 2.f;
            }
        }
    }
}

std::shared_ptr<NeuralNetController>
NeuralNetLayerWeightsCrossover::neuralNetCrossover(std::shared_ptr<NeuralNetController> p1,
                                                   std::shared_ptr<NeuralNetController> p2, float f1, float f2) {
    auto better = f1 >= f2 ? p1 : p2;
    auto worse = f1 >= f2 ? p2 : p1;

    auto child = std::static_pointer_cast<NeuralNetController>(better->copy());

    auto caNN = child->getAccNeuralNet();
    auto waNN = worse->getAccNeuralNet();
    auto crNN = child->getRotNeuralNet();
    auto wrNN = worse->getRotNeuralNet();

    nnCross(caNN, waNN);
    nnCross(crNN, wrNN);

    return child;
}

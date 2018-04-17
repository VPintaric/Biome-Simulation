#include <helpers/RNG.h>
#include "minion/mutation_operators/controller/neural_net/NeuralNetGaussNoiseWeightsMutation.h"

NeuralNetGaussNoiseWeightsMutation::NeuralNetGaussNoiseWeightsMutation() : mean(0.f), variance(0.f), distr(0.f, 0.f) {

}

void NeuralNetGaussNoiseWeightsMutation::configureFromJSON(rjs::Value &root) {
    const char * MEAN = "mean";
    const char * VARIANCE = "variance";

    if(root.HasMember(MEAN) && root[MEAN].IsFloat()){
        mean = root[MEAN].GetFloat();
    }

    if(root.HasMember(VARIANCE) && root[VARIANCE].IsFloat()){
        variance = root[VARIANCE].GetFloat();
    }

    distr = std::uniform_real_distribution<float>(mean, variance);
}

void NeuralNetGaussNoiseWeightsMutation::neuralNetMutation(std::shared_ptr<NeuralNetController> i) {
    auto nn = i->getNeuralNet();

    for(int layer = 0; layer < nn->weights.size(); layer++){
        auto w = nn->weights[layer];
        auto b = nn->bias[layer];
        for(int c = 0; c < w->cols(); c++){
            b->operator()(0, c) += distr(RNG::get());
            for(int r = 0; r < w->rows(); r++){
                w->operator()(r, c) += distr(RNG::get());
            }
        }
    }
}

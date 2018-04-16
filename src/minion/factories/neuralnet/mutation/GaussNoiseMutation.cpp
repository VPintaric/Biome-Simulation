#include <state/State.h>
#include <state/Log.h>
#include <helpers/RNG.h>
#include "minion/factories/neuralnet/mutation/GaussNoiseMutation.h"

GaussNoiseMutation::GaussNoiseMutation() : variance(5.f){

}

GaussNoiseMutation::~GaussNoiseMutation() = default;

void GaussNoiseMutation::configureFromJSON(rjs::Value &root) {
    const char * VARIANCE = "gauss_variance";

    if(root.HasMember(VARIANCE) && root[VARIANCE].IsFloat()){
        variance = root[VARIANCE].GetFloat();
    }
}

void GaussNoiseMutation::mutate(std::shared_ptr<NeuralNet> nn) {
    std::normal_distribution gauss(0.f, variance);

    for(int layer = 0; layer < nn->weights.size(); layer++){
        auto w = nn->weights[layer];
        auto b = nn->bias[layer];
        for(int c = 0; c < w->cols(); c++){
            b->operator()(0, c) += gauss(RNG::get());
            for(int r = 0; r < w->rows(); r++){
                w->operator()(r, c) += gauss(RNG::get());
            }
        }
    }
}

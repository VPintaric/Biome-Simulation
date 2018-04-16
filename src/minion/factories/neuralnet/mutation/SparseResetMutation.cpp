#include <state/State.h>
#include <state/Log.h>
#include <helpers/RNG.h>
#include "minion/factories/neuralnet/mutation/SparseResetMutation.h"

SparseResetMutation::SparseResetMutation() : resetProb(1e-3f), gaussVar(30.f) {

}

SparseResetMutation::~SparseResetMutation() = default;

void SparseResetMutation::configureFromJSON(rjs::Value &root) {
    const char * RESET_PROBABILITY = "reset_probability";
    const char * RESET_VALUE_GAUSS_VARIANCE = "reset_gauss_variance";

    if(root.HasMember(RESET_PROBABILITY) && root[RESET_PROBABILITY].IsFloat()){
        resetProb = root[RESET_PROBABILITY].GetFloat();
    }

    if(root.HasMember(RESET_VALUE_GAUSS_VARIANCE) && root[RESET_VALUE_GAUSS_VARIANCE].IsFloat()){
        gaussVar = root[RESET_VALUE_GAUSS_VARIANCE].GetFloat();
    }
}

void SparseResetMutation::mutate(std::shared_ptr<NeuralNet> nn) {
    std::uniform_real_distribution<float> uniform(0.f, 1.f);
    std::normal_distribution<float> normal(0.f, gaussVar);

    for(int layer = 0; layer < nn->weights.size(); layer++){
        auto w = nn->weights[layer];
        auto b = nn->bias[layer];
        for(int c = 0; c < w->cols(); c++){
            if(uniform(RNG::get()) <= resetProb){
                b->operator()(0, c) = normal(RNG::get());
            }
            for(int r = 0; r < w->rows(); r++){
                if(uniform(RNG::get()) <= resetProb){
                    w->operator()(r, c) = normal(RNG::get());
                }
            }
        }
    }
}

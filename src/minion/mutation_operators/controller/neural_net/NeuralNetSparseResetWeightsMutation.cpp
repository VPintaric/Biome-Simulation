#include <helpers/RNG.h>
#include "minion/mutation_operators/controller/neural_net/NeuralNetSparseResetWeightsMutation.h"

NeuralNetSparseResetWeightsMutation::NeuralNetSparseResetWeightsMutation() : weightProbability(0.f), biasProbability(0.f),
                                                                             variance(0.f), mean(0.f),
                                                                            distr(0.f, 0.f){

}

void NeuralNetSparseResetWeightsMutation::neuralNetMutation(std::shared_ptr<NeuralNetController> i) {
    auto nn = i->getNeuralNet();

    std::uniform_real_distribution<float> coinFlip(0.f, 1.f);

    for(int layer = 0; layer < nn->weights.size(); layer++){
        auto w = nn->weights[layer];
        auto b = nn->bias[layer];
        for(int c = 0; c < w->cols(); c++){
            if(coinFlip(RNG::get()) < biasProbability){
                b->operator()(0, c) = distr(RNG::get());
            }
            for(int r = 0; r < w->rows(); r++){
                if(coinFlip(RNG::get()) < weightProbability){
                    w->operator()(r, c) = distr(RNG::get());
                }
            }
        }
    }
}

void NeuralNetSparseResetWeightsMutation::configureFromJSON(rjs::Value &root) {
    const char * WEIGHT_RESET_PROBABILITY = "weight_reset_probability";
    const char * BIAS_RESET_PROBABILITY = "bias_reset_probability";
    const char * MEAN = "mean";
    const char * VARIANCE = "variance";

    NeuralNetControllerMutation::configureFromJSON(root);

    if(root.HasMember(WEIGHT_RESET_PROBABILITY) && root[WEIGHT_RESET_PROBABILITY].IsFloat()){
        weightProbability = root[WEIGHT_RESET_PROBABILITY].GetFloat();
    }

    if(root.HasMember(BIAS_RESET_PROBABILITY) && root[BIAS_RESET_PROBABILITY].IsFloat()){
        biasProbability = root[BIAS_RESET_PROBABILITY].GetFloat();
    }

    if(root.HasMember(MEAN) && root[MEAN].IsFloat()){
        mean = root[MEAN].GetFloat();
    }

    if(root.HasMember(VARIANCE) && root[VARIANCE].IsFloat()){
        variance = root[VARIANCE].GetFloat();
    }

    distr = std::uniform_real_distribution<float>(mean, variance);
}

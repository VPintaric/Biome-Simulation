#include <state/State.h>
#include <state/Log.h>
#include <helpers/RNG.h>
#include "minion/factories/neuralnet/crossover/InterleaveLayersCrossover.h"

InterleaveLayersCrossover::InterleaveLayersCrossover() = default;
InterleaveLayersCrossover::~InterleaveLayersCrossover() = default;

void InterleaveLayersCrossover::configureFromJSON(rjs::Value &root) {
    // no attributes to configure
}

void InterleaveLayersCrossover::crossover(std::shared_ptr<NeuralNet> p1, std::shared_ptr<NeuralNet> p2,
                                    std::shared_ptr<NeuralNet> c) {
    std::uniform_real_distribution<float> uniform(0.f, 1.f);

    for(int layer = 0; layer < c->weights.size(); layer++){
        if(uniform(RNG::get()) < 0.5f){
            *c->weights[layer] = *p1->weights[layer];
        } else {
            *c->weights[layer] = *p2->weights[layer];
        }

        if(uniform(RNG::get()) < 0.5f){
            *c->bias[layer] = *p1->bias[layer];
        } else {
            *c->bias[layer] = *p2->bias[layer];
        }
    }
}

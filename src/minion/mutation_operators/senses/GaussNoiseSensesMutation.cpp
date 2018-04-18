#include <helpers/RNG.h>
#include "minion/mutation_operators/senses/GaussNoiseSensesMutation.h"

GaussNoiseSensesMutation::GaussNoiseSensesMutation() : mean(0.f), variance(0.f), distr(0.f, 0.f) {

}

void GaussNoiseSensesMutation::configureFromJSON(rjs::Value &root) {
    const char * MEAN = "mean";
    const char * VARIANCE = "variance";

    if(root.HasMember(MEAN) && root[MEAN].IsFloat()){
        mean = root[MEAN].GetFloat();
    }

    if(root.HasMember(VARIANCE) && root[VARIANCE].IsFloat()){
        variance = root[VARIANCE].GetFloat();
    }

    distr = std::normal_distribution<float>(mean, variance);
}

void GaussNoiseSensesMutation::mutate(std::shared_ptr<MinionSenses> i) {
    i->setMaxSenseDistance(i->getMaxSenseDistance() + distr(RNG::get()));
}
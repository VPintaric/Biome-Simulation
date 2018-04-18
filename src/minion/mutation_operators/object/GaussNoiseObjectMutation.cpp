#include <helpers/RNG.h>
#include "minion/mutation_operators/object/GaussNoiseObjectMutation.h"

GaussNoiseObjectMutation::GaussNoiseObjectMutation() : radiusMean(0.f), radiusVariance(0.f), radiusDistr(0.f, 0.f),
                                                        colorMean(0.f), colorVariance(0.f), colorDistr(0.f, 0.f){

}

void GaussNoiseObjectMutation::configureFromJSON(rjs::Value &root) {
    const char * RADIUS_MEAN = "radius_mean";
    const char * RADIUS_VARIANCE = "radius_variance";
    const char * COLOR_MEAN = "color_mean";
    const char * COLOR_VARIANCE = "color_variance";

    if(root.HasMember(RADIUS_MEAN) && root[RADIUS_MEAN].IsFloat()){
        radiusMean = root[RADIUS_MEAN].GetFloat();
    }

    if(root.HasMember(RADIUS_VARIANCE) && root[RADIUS_VARIANCE].IsFloat()){
        radiusVariance = root[RADIUS_VARIANCE].GetFloat();
    }

    if(root.HasMember(COLOR_MEAN) && root[COLOR_MEAN].IsFloat()){
        colorMean = root[COLOR_MEAN].GetFloat();
    }

    if(root.HasMember(COLOR_VARIANCE) && root[COLOR_VARIANCE].IsFloat()){
        colorVariance = root[COLOR_VARIANCE].GetFloat();
    }

    radiusDistr = std::normal_distribution<float>(radiusMean, radiusVariance);
    colorDistr = std::normal_distribution<float>(colorMean, colorVariance);
}

void GaussNoiseObjectMutation::mutate(std::shared_ptr<MinionObject> i) {
    auto c = i->getSkinColor();
    c = glm::vec4(c.x + colorDistr(RNG::get()), c.y + colorDistr(RNG::get()), c.z + colorDistr(RNG::get()), 1.f);
    i->setSkinColor(c);

    i->setRadius(i->getRadius() + radiusDistr(RNG::get()));
}
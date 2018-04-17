#ifndef BIOME_SIMULATION_GAUSSNOISEOBJECTMUTATION_H
#define BIOME_SIMULATION_GAUSSNOISEOBJECTMUTATION_H


#include <random>
#include <minion/mutation_operators/object/MinionObjectMutation.h>

class GaussNoiseObjectMutation : public MinionObjectMutation {
private:
    float radiusMean, radiusVariance, colorMean, colorVariance;
    std::uniform_real_distribution<float> radiusDistr, colorDistr;

public:
    GaussNoiseObjectMutation();

    void configureFromJSON(rjs::Value &root) override;

    void mutate(std::shared_ptr<MinionObject> i) override;
};


#endif //BIOME_SIMULATION_GAUSSNOISEOBJECTMUTATION_H

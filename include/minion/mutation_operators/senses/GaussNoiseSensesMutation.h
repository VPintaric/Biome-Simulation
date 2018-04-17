#ifndef BIOME_SIMULATION_GAUSSNOISESENSESMUTATION_H
#define BIOME_SIMULATION_GAUSSNOISESENSESMUTATION_H


#include <minion/mutation_operators/senses/MinionSensesMutation.h>
#include <random>

class GaussNoiseSensesMutation : public MinionSensesMutation {
private:
    float mean, variance;
    std::uniform_real_distribution<float> distr;

public:
    GaussNoiseSensesMutation();

    void configureFromJSON(rjs::Value &root) override;

    void mutate(std::shared_ptr<MinionSenses> i) override;

};


#endif //BIOME_SIMULATION_GAUSSNOISESENSESMUTATION_H

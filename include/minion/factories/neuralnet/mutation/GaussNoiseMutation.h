#ifndef BIOME_SIMULATION_GAUSSNOISEMUTATION_H
#define BIOME_SIMULATION_GAUSSNOISEMUTATION_H


#include <minion/factories/neuralnet/mutation/NeuralNetMutation.h>

class GaussNoiseMutation : public NeuralNetMutation {
public:
    GaussNoiseMutation();
    virtual ~GaussNoiseMutation();

    void configureFromJSON(rjs::Value &root) override;

    void mutate(std::shared_ptr<NeuralNet> nn) override;

private:
    float variance;
};


#endif //BIOME_SIMULATION_GAUSSNOISEMUTATION_H

#ifndef BIOME_SIMULATION_SPARSERESETMUTATION_H
#define BIOME_SIMULATION_SPARSERESETMUTATION_H


#include <minion/factories/neuralnet/mutation/NeuralNetMutation.h>

class SparseResetMutation : public NeuralNetMutation {
public:
    SparseResetMutation();
    virtual ~SparseResetMutation();

    void configureFromJSON(rjs::Value &root) override;

    void mutate(std::shared_ptr<NeuralNet> nn) override;

private:
    float resetProb, gaussVar;
};


#endif //BIOME_SIMULATION_SPARSERESETMUTATION_H

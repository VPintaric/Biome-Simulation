#ifndef BIOME_SIMULATION_DECISIONTREECONTROLLERMUTATION_H
#define BIOME_SIMULATION_DECISIONTREECONTROLLERMUTATION_H


#include <minion/mutation_operators/controller/MinionControllerMutation.h>

class DecisionTreeControllerMutation : public MinionControllerMutation {
public:
    void mutate(std::shared_ptr<MinionController> i) override;

    virtual void decisionTreeMutation(std::shared_ptr<DecisionTreeController> i) = 0;
};


#endif //BIOME_SIMULATION_DECISIONTREECONTROLLERMUTATION_H

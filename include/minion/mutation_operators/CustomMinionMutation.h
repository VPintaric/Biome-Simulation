#ifndef BIOME_SIMULATION_CUSTOMMINIONMUTATION_H
#define BIOME_SIMULATION_CUSTOMMINIONMUTATION_H


#include <minion/mutation_operators/MinionMutation.h>
#include <minion/mutation_operators/controller/MinionControllerMutation.h>
#include <minion/mutation_operators/senses/MinionSensesMutation.h>
#include <minion/mutation_operators/object/MinionObjectMutation.h>

class CustomMinionMutation : public MinionMutation {
    std::vector<std::shared_ptr<MinionControllerMutation>> controllerOps;
    std::vector<std::shared_ptr<MinionSensesMutation>> sensesOps;
    std::vector<std::shared_ptr<MinionObjectMutation>> objectOps;

    std::vector<float> controllerOpsProbs, sensesOpsProbs, objectOpsProbs;

    void addObjectMutationOp(rjs::Value &root);
    void addSensesMutationOp(rjs::Value &root);
    void addControllerMutationOp(rjs::Value &root);
    void fixProbabilities(std::vector<float> &probs);
    int getRandomIndexFromProbs(std::vector<float> &probs);

public:
    void configureFromJSON(rjs::Value &root) override;

    void mutate(std::shared_ptr<Minion> m) override;

};


#endif //BIOME_SIMULATION_CUSTOMMINIONMUTATION_H

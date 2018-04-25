#ifndef BIOME_SIMULATION_DTRANDOMIZENODES_H
#define BIOME_SIMULATION_DTRANDOMIZENODES_H

#include <minion/mutation_operators/controller/decision_tree/DecisionTreeControllerMutation.h>
#include <minion/controllers/decision_tree/DecisionTreeController.h>

class DTRandomizeNodes : public DecisionTreeControllerMutation {
private:
    int minDepth;
    float pMutateNode;

    void mutateTree(std::shared_ptr<DTNode> node, int d, std::uniform_int_distribution<int> fd,
                    std::uniform_int_distribution<int> rd);

public:
    DTRandomizeNodes();

    void configureFromJSON(rjs::Value &root) override;

    void decisionTreeMutation(std::shared_ptr<DecisionTreeController> i) override;
};


#endif //BIOME_SIMULATION_DTRANDOMIZENODES_H

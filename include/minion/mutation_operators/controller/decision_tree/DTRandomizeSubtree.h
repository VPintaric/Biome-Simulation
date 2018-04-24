#ifndef BIOME_SIMULATION_DTRANDOMIZESUBTREE_H
#define BIOME_SIMULATION_DTRANDOMIZESUBTREE_H


#include <minion/controllers/decision_tree/DecisionTreeController.h>
#include "DecisionTreeControllerMutation.h"

class DTRandomizeSubtree : public DecisionTreeControllerMutation {
private:
    int minDepth, maxDepth, subtreeMaxDepth;

    bool modifyTree(std::shared_ptr<DTNode> node, int d,
                    std::uniform_int_distribution<int> fd, std::uniform_int_distribution<int> rd);

    std::shared_ptr<DTNode> generateRandomSubTree(int d, std::uniform_int_distribution<int> fd,
                          std::uniform_int_distribution<int> rd);

public:
    DTRandomizeSubtree();

    void configureFromJSON(rjs::Value &root) override;

    void decisionTreeMutation(std::shared_ptr<DecisionTreeController> i) override;
};


#endif //BIOME_SIMULATION_DTRANDOMIZESUBTREE_H

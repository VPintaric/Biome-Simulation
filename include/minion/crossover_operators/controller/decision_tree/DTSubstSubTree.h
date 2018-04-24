#ifndef BIOME_SIMULATION_DTSUBSTSUBTREE_H
#define BIOME_SIMULATION_DTSUBSTSUBTREE_H


#include "DecisionTreeControllerCrossover.h"

class DTSubstSubTree : public DecisionTreeControllerCrossover {
private:

    float pModifyBetterParent;
    int minDepthCrossPoint, maxDepthCrossPoint;

    bool modifyTree(std::shared_ptr<DTNode> node, std::shared_ptr<DecisionTree> source, int d);

public:
    DTSubstSubTree();

    void configureFromJSON(rjs::Value &root) override;

    std::shared_ptr<DecisionTreeController> decisionTreeCrossover(std::shared_ptr<DecisionTreeController> p1,
                                                                  std::shared_ptr<DecisionTreeController> p2,
                                                                  float f1, float f2) override;
};


#endif //BIOME_SIMULATION_DTSUBSTSUBTREE_H

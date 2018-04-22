#ifndef BIOME_SIMULATION_DECISIONTREE_H
#define BIOME_SIMULATION_DECISIONTREE_H


#include <copyable/Copyable.h>
#include <set>
#include "DTNode.h"

class DTNode;

class DecisionTree : public Copyable<DecisionTree> {
private:
    std::shared_ptr<DTNode> root;

    int nFacts, nResults;

public:

    DecisionTree(int nUniqueFacts, int nUniqueResults);

    std::shared_ptr<DecisionTree> copy() override;

    int evaluate(const std::set<int>& facts);
};


#endif //BIOME_SIMULATION_DECISIONTREE_H

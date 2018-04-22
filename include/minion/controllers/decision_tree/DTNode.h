#ifndef BIOME_SIMULATION_DTNODE_H
#define BIOME_SIMULATION_DTNODE_H


#include <copyable/Copyable.h>
#include "DecisionTree.h"

class DecisionTree;

class DTNode : public Copyable<DTNode> {
public:
    std::shared_ptr<DTNode> copy() override;

    virtual int evaluate(const std::set<int>& facts) = 0;

    std::weak_ptr<DTNode> parent;
    std::weak_ptr<DecisionTree> tree;
};


#endif //BIOME_SIMULATION_DTNODE_H

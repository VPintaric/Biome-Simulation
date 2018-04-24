#ifndef BIOME_SIMULATION_DTNODE_H
#define BIOME_SIMULATION_DTNODE_H


#include <copyable/Copyable.h>
#include "DecisionTree.h"

class DecisionTree;

class DTNode : public Copyable<DTNode> {
public:
    virtual int evaluate(const std::set<int>& facts) = 0;

    virtual int randomEvaluate() = 0;

    virtual void appendToStream(std::stringstream &ss) = 0;

    virtual void getFromStream(std::stringstream &ss) = 0;
};


#endif //BIOME_SIMULATION_DTNODE_H

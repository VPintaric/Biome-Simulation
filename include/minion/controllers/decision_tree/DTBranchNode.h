#ifndef BIOME_SIMULATION_DTBRANCHNODE_H
#define BIOME_SIMULATION_DTBRANCHNODE_H


#include "DTNode.h"

class DTBranchNode : public DTNode {
public:
    int factToQuery;

    std::shared_ptr<DTNode> left, right;

    explicit DTBranchNode(int factToQuery);

    std::shared_ptr<DTNode> copy() override;

    int evaluate(const std::set<int> &facts) override;

    int randomEvaluate() override;

    void appendToStream(std::stringstream &ss) override;

    void getFromStream(std::stringstream &ss) override;
};


#endif //BIOME_SIMULATION_DTBRANCHNODE_H

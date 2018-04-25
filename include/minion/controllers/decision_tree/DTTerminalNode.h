#ifndef BIOME_SIMULATION_DTTERMINALNODE_H
#define BIOME_SIMULATION_DTTERMINALNODE_H


#include "DTNode.h"

class DTTerminalNode : public DTNode {
public:
    int returnValue;

    explicit DTTerminalNode(int returnValue);

    std::shared_ptr<DTNode> copy() override;

    int evaluate(const std::set<int> &facts) override;

    int randomEvaluate() override;

    void appendToStream(std::stringstream &ss) override;

    void getFromStream(std::stringstream &ss) override;
};


#endif //BIOME_SIMULATION_DTTERMINALNODE_H

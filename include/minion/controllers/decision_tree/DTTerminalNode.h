#ifndef BIOME_SIMULATION_DTTERMINALNODE_H
#define BIOME_SIMULATION_DTTERMINALNODE_H


#include "DTNode.h"

class DTTerminalNode : public DTNode {
private:
    int returnValue;

public:
    explicit DTTerminalNode(int returnValue);

    std::shared_ptr<DTNode> copy() override;

    int evaluate(const std::set<int> &facts) override;
};


#endif //BIOME_SIMULATION_DTTERMINALNODE_H

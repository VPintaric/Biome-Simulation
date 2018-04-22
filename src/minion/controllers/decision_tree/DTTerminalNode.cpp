#include "minion/controllers/decision_tree/DTTerminalNode.h"

DTTerminalNode::DTTerminalNode(int returnValue) : returnValue(returnValue) {

}

std::shared_ptr<DTNode> DTTerminalNode::copy() {
    return std::shared_ptr<DTNode>(new DTTerminalNode(returnValue));
}

int DTTerminalNode::evaluate(const std::set<int> &facts) {
    return returnValue;
}

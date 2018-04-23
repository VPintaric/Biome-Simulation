#include "minion/controllers/decision_tree/DTTerminalNode.h"
#include <sstream>

DTTerminalNode::DTTerminalNode(int returnValue) : returnValue(returnValue) {

}

std::shared_ptr<DTNode> DTTerminalNode::copy() {
    return std::shared_ptr<DTNode>(new DTTerminalNode(returnValue));
}

int DTTerminalNode::evaluate(const std::set<int> &facts) {
    return returnValue;
}

void DTTerminalNode::appendToStream(std::stringstream &ss) {
    ss << "t " << returnValue << " ";
}

void DTTerminalNode::getFromStream(std::stringstream &ss) {
    // Nothing to do
}

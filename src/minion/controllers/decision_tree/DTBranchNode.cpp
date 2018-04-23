#include <minion/controllers/decision_tree/DTTerminalNode.h>
#include "minion/controllers/decision_tree/DTBranchNode.h"
#include <sstream>

DTBranchNode::DTBranchNode(int factToQuery) : factToQuery(factToQuery) {

}

std::shared_ptr<DTNode> DTBranchNode::copy() {
    auto newCopy = std::make_shared<DTBranchNode>(factToQuery);
    newCopy->left = left->copy();
    newCopy->left->parent = newCopy;
    newCopy->right = right->copy();
    newCopy->right->parent = newCopy;
    return newCopy;
}

int DTBranchNode::evaluate(const std::set<int> &facts) {
    if(facts.find(factToQuery) != facts.end()){
        return left->evaluate(facts);
    } else {
        return right->evaluate(facts);
    }
}

void DTBranchNode::appendToStream(std::stringstream &ss) {
    ss << "b " << factToQuery << " ";
    left->appendToStream(ss);
    right->appendToStream(ss);
}

void DTBranchNode::getFromStream(std::stringstream &ss) {
    std::string nodeType;
    int n;

    ss >> nodeType >> n;
    if(nodeType == "t"){
        left = std::make_shared<DTTerminalNode>(n);
    } else {
        left = std::make_shared<DTBranchNode>(n);
    }
    left->parent = std::shared_ptr<DTNode>(this);

    ss >> nodeType >> n;
    if(nodeType == "t"){
        right = std::make_shared<DTTerminalNode>(n);
    } else {
        right = std::make_shared<DTBranchNode>(n);
    }
    right->parent = std::shared_ptr<DTNode>(this);
}

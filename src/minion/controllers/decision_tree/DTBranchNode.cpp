#include "minion/controllers/decision_tree/DTBranchNode.h"

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
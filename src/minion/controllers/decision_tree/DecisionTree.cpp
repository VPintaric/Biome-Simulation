#include "minion/controllers/decision_tree/DecisionTree.h"

DecisionTree::DecisionTree(int nUniqueFacts, int nUniqueResults) : nFacts(nUniqueFacts),
                                                                   nResults(nUniqueResults) {

}

int DecisionTree::evaluate(const std::set<int> &facts) {
    return root->evaluate(facts);
}

std::shared_ptr<DecisionTree> DecisionTree::copy() {
    auto newCopy = std::make_shared<DecisionTree>(nFacts, nResults);
    newCopy->root = root->copy();

    return newCopy;
}


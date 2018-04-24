#include <sstream>
#include <minion/controllers/decision_tree/DTTerminalNode.h>
#include <minion/controllers/decision_tree/DTBranchNode.h>

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

void DecisionTree::persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) {
    root.AddMember(rjs::StringRef(N_FACTS), rjs::Value(nFacts), alloc);
    root.AddMember(rjs::StringRef(N_RESULTS), rjs::Value(nResults), alloc);

    rjs::Value nodes(rjs::kStringType);
    std::stringstream ss;
    this->root->appendToStream(ss);
    std::string s = ss.str();
    nodes.SetString(s.c_str(), static_cast<rapidjson::SizeType>(s.size()), alloc);
    root.AddMember(rjs::StringRef(NODES), nodes, alloc);
}

void DecisionTree::initFromJSON(rjs::Value &root) {
    nFacts = root[N_FACTS].GetInt();
    nResults = root[N_RESULTS].GetInt();

    std::string nodes = root[NODES].GetString();
    std::stringstream ss(nodes);

    std::string nodeType;
    int n;
    ss >> nodeType >> n;

    if(nodeType == "t"){
        this->root = std::make_shared<DTTerminalNode>(n);
    } else {
        this->root = std::make_shared<DTBranchNode>(n);
    }

    this->root->getFromStream(ss);
}

void DecisionTree::randomPruningRec(std::shared_ptr<DTNode> node, int d, int maxBranchDepth) {
    auto branchNode = std::dynamic_pointer_cast<DTBranchNode>(node);
    if(branchNode != nullptr){
        if(d == maxBranchDepth){
            branchNode->left = std::make_shared<DTTerminalNode>(branchNode->left->randomEvaluate());
            branchNode->right = std::make_shared<DTTerminalNode>(branchNode->right->randomEvaluate());
        } else {
            randomPruningRec(branchNode->left, d + 1, maxBranchDepth);
            randomPruningRec(branchNode->right, d + 1, maxBranchDepth);
        }
    }
}

void DecisionTree::randomTreePruning(int maxDepth) {
    randomPruningRec(root, 0, maxDepth - 1);
}
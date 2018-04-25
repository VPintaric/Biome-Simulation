#include <helpers/RNG.h>
#include <minion/controllers/decision_tree/DTTerminalNode.h>
#include "minion/mutation_operators/controller/decision_tree/DTRandomizeNodes.h"

DTRandomizeNodes::DTRandomizeNodes() : pMutateNode(1e-4f), minDepth(1) {

}

void DTRandomizeNodes::configureFromJSON(rjs::Value &root) {
    const char * MIN_DEPTH = "min_depth";
    const char * P_MUTATE_NODE = "probability_mutate_node";

    if(root.HasMember(MIN_DEPTH) && root[MIN_DEPTH].IsInt()){
        minDepth = root[MIN_DEPTH].GetInt();
    }

    if(root.HasMember(P_MUTATE_NODE) && root[P_MUTATE_NODE].IsFloat()){
        pMutateNode = root[P_MUTATE_NODE].GetFloat();
    }
}

void DTRandomizeNodes::mutateTree(std::shared_ptr<DTNode> node, int d, std::uniform_int_distribution<int> fd,
                                  std::uniform_int_distribution<int> rd) {
    auto branchNode = std::dynamic_pointer_cast<DTBranchNode>(node);
    if(branchNode != nullptr){
        if(d >= minDepth && RNG::roll() <= pMutateNode){
            branchNode->factToQuery = fd(RNG::get());
        }
        mutateTree(branchNode->left, d + 1, fd, rd);
        mutateTree(branchNode->right, d + 1, fd, rd);
    } else {
        auto terminalNode = std::dynamic_pointer_cast<DTTerminalNode>(node);
        if(d >= minDepth && RNG::roll() <= pMutateNode){
            terminalNode->returnValue = rd(RNG::get());
        }
    }
}

void DTRandomizeNodes::decisionTreeMutation(std::shared_ptr<DecisionTreeController> i) {
    std::uniform_int_distribution<int> fd(0, DTConst::__FACT_MAX__);
    std::uniform_int_distribution<int> accRd(0, DTConst::__ACCTYPE_MAX__);
    std::uniform_int_distribution<int> rotRd(0, DTConst::__ROTTYPE_MAX__);

    mutateTree(i->accDecTree->root, 0, fd, accRd);
    mutateTree(i->rotDecTree->root, 0, fd, rotRd);
}

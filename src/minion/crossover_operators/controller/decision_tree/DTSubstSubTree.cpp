#include <helpers/RNG.h>
#include "minion/crossover_operators/controller/decision_tree/DTSubstSubTree.h"

DTSubstSubTree::DTSubstSubTree() : pModifyBetterParent(0.5f), minDepthCrossPoint(1) {

}

void DTSubstSubTree::configureFromJSON(rjs::Value &root) {
    const char * P_MODIFY_BETTER = "p_modify_better_parent";
    const char * MIN_DEPTH = "min_depth_cross_point";
    const char * MAX_DEPTH = "max_depth_cross_point";

    if(root.HasMember(P_MODIFY_BETTER) && root[P_MODIFY_BETTER].IsFloat()) {
        pModifyBetterParent = root[P_MODIFY_BETTER].GetFloat();
    }

    if(root.HasMember(MIN_DEPTH) && root[MIN_DEPTH].IsInt()){
        minDepthCrossPoint = root[MIN_DEPTH].GetInt();
    }

    if(root.HasMember(MAX_DEPTH) && root[MAX_DEPTH].IsInt()){
        maxDepthCrossPoint = root[MAX_DEPTH].GetInt();
    }
}

bool DTSubstSubTree::modifyTree(std::shared_ptr<DTNode> node, std::shared_ptr<DecisionTree> source, int d) {
    auto branchNode = std::dynamic_pointer_cast<DTBranchNode>(node);
    if(branchNode != nullptr){
        auto checkLeftFirst = static_cast<bool>(RNG::get()() % 2);
        auto& first = checkLeftFirst ? branchNode->left : branchNode->right;
        auto& second = checkLeftFirst ? branchNode->right : branchNode->left;

        if(d + 1 >= minDepthCrossPoint){
            float probToModifyHere = (float) (d + 1 - minDepthCrossPoint) / (maxDepthCrossPoint - minDepthCrossPoint);
            if(RNG::roll() <= probToModifyHere){
                first = source->root->copy();
                return true;
            } else if(RNG::roll() <= probToModifyHere){
                second = source->root->copy();
                return true;
            }
        }

        if(modifyTree(first, source, d + 1)){
            return true;
        }
        if(modifyTree(second, source, d + 1)){
            return true;
        }
    }
    return false;
}

std::shared_ptr<DecisionTreeController>
DTSubstSubTree::decisionTreeCrossover(std::shared_ptr<DecisionTreeController> p1,
                                          std::shared_ptr<DecisionTreeController> p2, float f1, float f2) {
    auto worse = f1 < f2 ? p1 : p2;
    auto better = f1 >= f2 ? p1 : p2;

    std::shared_ptr<DecisionTreeController> child, source;

    if(RNG::roll() <= pModifyBetterParent){
        child = std::static_pointer_cast<DecisionTreeController>(better->copy());
        source = worse;
    } else {
        child = std::static_pointer_cast<DecisionTreeController>(worse->copy());
        source = better;
    }

    modifyTree(child->accDecTree->root, source->accDecTree, 0);
    modifyTree(child->rotDecTree->root, source->rotDecTree, 0);
    child->pruneTrees();

    return child;
}

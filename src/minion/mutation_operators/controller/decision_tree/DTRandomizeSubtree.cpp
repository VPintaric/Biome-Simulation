#include <minion/mutation_operators/controller/decision_tree/DTRandomizeSubtree.h>
#include <helpers/RNG.h>
#include <minion/controllers/decision_tree/DTTerminalNode.h>

DTRandomizeSubtree::DTRandomizeSubtree() : minDepth(5), maxDepth(10) {

}

void DTRandomizeSubtree::configureFromJSON(rjs::Value &root) {
    const char * MIN_DEPTH = "min_depth";
    const char * MAX_DEPTH = "max_depth";
    const char * SUBTREE_MAX_DEPTH = "subtree_max_depth";

    if(root.HasMember(MIN_DEPTH) && root[MIN_DEPTH].IsInt()){
        minDepth = root[MIN_DEPTH].GetInt();
    }

    if(root.HasMember(MAX_DEPTH) && root[MAX_DEPTH].IsInt()){
        maxDepth = root[MAX_DEPTH].GetInt();
    }

    if(root.HasMember(SUBTREE_MAX_DEPTH) && root[SUBTREE_MAX_DEPTH].IsInt()){
        subtreeMaxDepth = root[SUBTREE_MAX_DEPTH].GetInt();
    }
}

std::shared_ptr<DTNode> DTRandomizeSubtree::generateRandomSubTree(int d, std::uniform_int_distribution<int> fd,
                                              std::uniform_int_distribution<int> rd){
    float pForTerminal = (float) d / subtreeMaxDepth;

    if(RNG::roll() <= pForTerminal){
        return std::make_shared<DTTerminalNode>(rd(RNG::get()));
    } else {
        auto newBranchNode = std::make_shared<DTBranchNode>(fd(RNG::get()));
        newBranchNode->left = generateRandomSubTree(d + 1, fd, rd);
        newBranchNode->right = generateRandomSubTree(d + 1, fd, rd);
        return newBranchNode;
    }
}

bool DTRandomizeSubtree::modifyTree(std::shared_ptr<DTNode> node, int d,
                                    std::uniform_int_distribution<int> fd, std::uniform_int_distribution<int> rd) {
    auto branchNode = std::dynamic_pointer_cast<DTBranchNode>(node);
    if(branchNode != nullptr){
        auto checkLeftFirst = static_cast<bool>(RNG::get()() % 2);
        auto& first = checkLeftFirst ? branchNode->left : branchNode->right;
        auto& second = checkLeftFirst ? branchNode->right : branchNode->left;

        if(d + 1 >= minDepth){
            float probToModifyHere = (float) (d + 1 - minDepth) / (maxDepth - minDepth);
            if(RNG::roll() <= probToModifyHere){
                first = generateRandomSubTree(0, fd, rd);
                return true;
            } else if(RNG::roll() <= probToModifyHere){
                second = generateRandomSubTree(0, fd, rd);
                return true;
            }
        }

        if(modifyTree(first, d + 1, fd, rd)){
            return true;
        }
        if(modifyTree(second, d + 1, fd, rd)){
            return true;
        }
    }
    return false;
}

void DTRandomizeSubtree::decisionTreeMutation(std::shared_ptr <DecisionTreeController> i) {
    std::uniform_int_distribution<int> factsDistr(0, DTConst::__FACT_MAX__);
    std::uniform_int_distribution<int> accDistr(0, DTConst::__ACCTYPE_MAX__);
    std::uniform_int_distribution<int> rotDistr(0, DTConst::__ROTTYPE_MAX__);

    modifyTree(i->accDecTree->root, 0, factsDistr, accDistr);
    modifyTree(i->rotDecTree->root, 0, factsDistr, rotDistr);

    i->pruneTrees();
}

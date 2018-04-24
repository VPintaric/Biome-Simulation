#ifndef BIOME_SIMULATION_DECISIONTREECONTROLLER_H
#define BIOME_SIMULATION_DECISIONTREECONTROLLER_H


#include <minion/controllers/MinionController.h>
#include <set>
#include <random>
#include "DTConst.h"
#include "DecisionTree.h"
#include "DTBranchNode.h"

class DecisionTreeController : public MinionController {
private:

    const char * ACC_TREE = "acceleration_tree";
    const char * ROT_TREE = "rotation_tree";
    const char * MAX_DEPTH = "max_depth";

    std::set<int> preprocess(std::vector<float> &senseData);

    std::shared_ptr<Minion> lockMinion();

    void initRandomTree(std::shared_ptr<DTBranchNode> node,
                        std::uniform_int_distribution<int>& factsDistr,
                        std::uniform_int_distribution<int>& resDistr, int d);

    std::weak_ptr<Minion> m;

    int maxTreeDepth;

public:
    std::shared_ptr<DecisionTree> accDecTree, rotDecTree;

    explicit DecisionTreeController(int maxTreeDepth);

    void initRandom();

    std::shared_ptr<MinionController> copy() override;

    std::vector<float> controlMinion(std::vector<float> senseData) override;

    void setMinion(std::shared_ptr<Minion> m) override;

    void persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) override;

    void initFromJSON(rjs::Value &root) override;

    void pruneTrees();
};


#endif //BIOME_SIMULATION_DECISIONTREECONTROLLER_H

#ifndef BIOME_SIMULATION_DECISIONTREECONTROLLER_H
#define BIOME_SIMULATION_DECISIONTREECONTROLLER_H


#include <minion/controllers/MinionController.h>
#include <set>
#include "DTConst.h"
#include "DecisionTree.h"

class DecisionTreeController : public MinionController {
private:

    std::set<int> preprocess(std::vector<float> &senseData);

    std::shared_ptr<Minion> lockMinion();

    std::weak_ptr<Minion> m;

    std::shared_ptr<DecisionTree> accDecTree, rotDecTree;

public:
    std::shared_ptr<MinionController> copy() override;

    std::vector<float> controlMinion(std::vector<float> senseData) override;

    void setMinion(std::shared_ptr<Minion> m) override;
};


#endif //BIOME_SIMULATION_DECISIONTREECONTROLLER_H

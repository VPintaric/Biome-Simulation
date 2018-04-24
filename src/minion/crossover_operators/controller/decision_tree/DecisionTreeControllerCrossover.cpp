#include <state/Log.h>
#include "minion/crossover_operators/controller/decision_tree/DecisionTreeControllerCrossover.h"

std::shared_ptr<MinionController>
DecisionTreeControllerCrossover::crossover(std::shared_ptr<MinionController> p1, std::shared_ptr<MinionController> p2,
                                           float f1, float f2) {
    auto dtFirstParent = std::dynamic_pointer_cast<DecisionTreeController>(p1);
    auto dtSecondParent = std::dynamic_pointer_cast<DecisionTreeController>(p2);

    if(dtFirstParent == nullptr || dtSecondParent == nullptr){
        Log().Get(logWARNING) << "Incompatible types given to decision tree crossover operator, returning a copy of "
                              "the first parent";
        return p1->copy();
    }

    return decisionTreeCrossover(dtFirstParent, dtSecondParent, f1, f2);
}

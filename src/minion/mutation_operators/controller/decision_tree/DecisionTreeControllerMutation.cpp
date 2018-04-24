#include <minion/controllers/decision_tree/DecisionTreeController.h>
#include <state/Log.h>
#include "minion/mutation_operators/controller/decision_tree/DecisionTreeControllerMutation.h"

void DecisionTreeControllerMutation::mutate(std::shared_ptr<MinionController> i) {
    auto controller = std::dynamic_pointer_cast<DecisionTreeController>(i);

    if(controller == nullptr){
        Log().Get(logWARNING) << "Uncompatible controller given to decision tree mutation operator, not doing anything";
        return;
    }

    decisionTreeMutation(controller);
}

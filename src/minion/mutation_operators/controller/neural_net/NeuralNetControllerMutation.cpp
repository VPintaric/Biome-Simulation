#include <state/Log.h>
#include "minion/mutation_operators/controller/neural_net/NeuralNetControllerMutation.h"

void NeuralNetControllerMutation::configureFromJSON(rjs::Value &root) {
    // Nothing to configure
}

void NeuralNetControllerMutation::mutate(std::shared_ptr<MinionController> i) {
    auto controller = std::dynamic_pointer_cast<NeuralNetController>(i);

    if(controller == nullptr){
        Log().Get(logWARNING) << "Uncompatible controller given to neural net mutation operator, not doing anything";
        return;
    }

    neuralNetMutation(controller);
}

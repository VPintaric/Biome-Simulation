#include <state/Log.h>
#include "minion/crossover_operators/controller/neural_net/NeuralNetControllerCrossover.h"

void NeuralNetControllerCrossover::configureFromJSON(rjs::Value &root) {
    // Nothing to configure
}

std::shared_ptr<MinionController>
NeuralNetControllerCrossover::crossover(std::shared_ptr<MinionController> p1, std::shared_ptr<MinionController> p2,
                                        float f1, float f2) {
    auto nnFirstParent = std::dynamic_pointer_cast<NeuralNetController>(p1);
    auto nnSecondParent = std::dynamic_pointer_cast<NeuralNetController>(p2);

    if(nnFirstParent == nullptr || nnSecondParent == nullptr){
        Log().Get(logWARNING) << "Incompatible types given to neural net crossover operator, returning a copy of the "
                "first parent";
        return p1->copy();
    }

    return neuralNetCrossover(nnFirstParent, nnSecondParent, f1, f2);
}

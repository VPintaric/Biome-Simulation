#include "minion/crossover_operators/controller/PickBetterControllerCrossover.h"

void PickBetterControllerCrossover::configureFromJSON(rjs::Value &root) {
    // Nothing to configure
}

std::shared_ptr<MinionController>
PickBetterControllerCrossover::crossover(std::shared_ptr<MinionController> p1, std::shared_ptr<MinionController> p2,
                                         float f1, float f2) {
    return f1 >= f2 ? p1->copy() : p2->copy();
}

#include "minion/crossover_operators/object/PickBetterObjectCrossover.h"

void PickBetterObjectCrossover::configureFromJSON(rjs::Value &root) {
    // Nothing to configure
}

std::shared_ptr<MinionObject>
PickBetterObjectCrossover::crossover(std::shared_ptr<MinionObject> p1, std::shared_ptr<MinionObject> p2, float f1,
                                     float f2) {
    return f1 >= f2 ? p1->copy() : p2->copy();
}

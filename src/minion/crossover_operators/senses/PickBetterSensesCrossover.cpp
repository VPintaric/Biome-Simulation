#include "minion/crossover_operators/senses/PickBetterSensesCrossover.h"

void PickBetterSensesCrossover::configureFromJSON(rjs::Value &root) {
    // Nothing to configure
}

std::shared_ptr<MinionSenses>
PickBetterSensesCrossover::crossover(std::shared_ptr<MinionSenses> p1, std::shared_ptr<MinionSenses> p2, float f1,
                                     float f2) {
    return f1 >= f2 ? p1->copy() : p2->copy();
}

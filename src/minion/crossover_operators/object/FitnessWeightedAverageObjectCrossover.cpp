#include "minion/crossover_operators/object/FitnessWeightedAverageObjectCrossover.h"

void FitnessWeightedAverageObjectCrossover::configureFromJSON(rjs::Value &root) {
    // Nothing to configure
}

std::shared_ptr<MinionObject>
FitnessWeightedAverageObjectCrossover::crossover(std::shared_ptr<MinionObject> p1, std::shared_ptr<MinionObject> p2,
                                                 float f1, float f2) {
    float sum = f1 + f2;

    auto child = p1->copy();

    child->setSkinColor((f1 * p1->getSkinColor() + f2 * p2->getSkinColor()) / sum);
    child->setRadius((f1 * p1->getRadius() + f2 * p2->getRadius()) / sum);

    return child;
}

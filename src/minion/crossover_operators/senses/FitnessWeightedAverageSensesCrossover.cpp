#include "minion/crossover_operators/senses/FitnessWeightedAverageSensesCrossover.h"

void FitnessWeightedAverageSensesCrossover::configureFromJSON(rjs::Value &root) {
    // Nothing to configure
}

std::shared_ptr<MinionSenses>
FitnessWeightedAverageSensesCrossover::crossover(std::shared_ptr<MinionSenses> p1, std::shared_ptr<MinionSenses> p2,
                                                 float f1, float f2) {
    float sum = f1 + f2;

    auto child = p1->copy();

    child->setMaxSenseDistance((f1 * p1->getMaxSenseDistance() + f2 * p2->getMaxSenseDistance()) / sum);

    return child;
}

#include "minion/crossover_operators/PickBetterMinionCrossover.h"

PickBetterMinionCrossover::PickBetterMinionCrossover() = default;

PickBetterMinionCrossover::~PickBetterMinionCrossover() = default;

void PickBetterMinionCrossover::configureFromJSON(rjs::Value &root) {
    // Nothing to configure
}

std::shared_ptr<Minion> PickBetterMinionCrossover::crossover(std::shared_ptr<Minion> m1, std::shared_ptr<Minion> m2) {
    return m1->getFitness() >= m2->getFitness() ? m1->copy() : m2->copy();
}

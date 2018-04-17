#include <random>
#include <helpers/RNG.h>
#include "minion/crossover_operators/PickRandomMinionCrossover.h"

void PickRandomMinionCrossover::configureFromJSON(rjs::Value &root) {
    // Nothing to configure
}

std::shared_ptr<Minion> PickRandomMinionCrossover::crossover(std::shared_ptr<Minion> m1, std::shared_ptr<Minion> m2) {
    std::uniform_int_distribution<> distr(0, 1);

    return distr(RNG::get()) ? m2->copy() : m1->copy();
}

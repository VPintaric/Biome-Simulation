#include "minion/fitness/TimeLivedFitness.h"

void TimeLivedFitness::configureFromJSON(rjs::Value &root) {
    // Nothing to configure...
}

void TimeLivedFitness::calculateFitness(std::shared_ptr<Minion> m) {
    m->setFitness(m->getTimeLived());
}

#include "minion/mutation_operators/DoNothingMinionMutation.h"

void DoNothingMinionMutation::configureFromJSON(rjs::Value &root) {
    // Nothing to configure
}

void DoNothingMinionMutation::mutate(std::shared_ptr<Minion> m) {
    // Nothing to do
}

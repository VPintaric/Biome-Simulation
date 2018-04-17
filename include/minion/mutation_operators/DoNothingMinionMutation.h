#ifndef BIOME_SIMULATION_DONOTHINGMINIONMUTATION_H
#define BIOME_SIMULATION_DONOTHINGMINIONMUTATION_H


#include <minion/mutation_operators/MinionMutation.h>

class DoNothingMinionMutation : public MinionMutation {
public:
    void configureFromJSON(rjs::Value &root) override;

    void mutate(std::shared_ptr<Minion> m) override;

};


#endif //BIOME_SIMULATION_DONOTHINGMINIONMUTATION_H

#ifndef BIOME_SIMULATION_HARDCODEDMINIONGENERATOR_H
#define BIOME_SIMULATION_HARDCODEDMINIONGENERATOR_H


#include <minion/factories/MinionGenerator.h>

class HardcodedMinionGenerator : public MinionGenerator {
public:
    void configureFromJSON(rjs::Value &root) override;

    std::shared_ptr<Minion> generateRandomMinion() override;

    std::shared_ptr<Minion> generateMinion() override;

};


#endif //BIOME_SIMULATION_HARDCODEDMINIONGENERATOR_H

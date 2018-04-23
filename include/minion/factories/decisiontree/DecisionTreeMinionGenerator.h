#ifndef BIOME_SIMULATION_DECISIONTREEMINIONGENERATOR_H
#define BIOME_SIMULATION_DECISIONTREEMINIONGENERATOR_H


#include <minion/factories/MinionGenerator.h>

class DecisionTreeMinionGenerator : public MinionGenerator {
private:
    int maxTreeDepth;

public:
    DecisionTreeMinionGenerator();

    void configureFromJSON(rjs::Value &root) override;

    std::shared_ptr<Minion> generateRandomMinion() override;

    std::shared_ptr<Minion> generateMinion() override;

};


#endif //BIOME_SIMULATION_DECISIONTREEMINIONGENERATOR_H

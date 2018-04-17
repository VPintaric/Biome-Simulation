#ifndef BIOME_SIMULATION_CUSTOMCROSSOVER_H
#define BIOME_SIMULATION_CUSTOMCROSSOVER_H


#include <minion/crossover_operators/controller/MinionControllerCrossover.h>
#include <minion/crossover_operators/senses/MinionSensesCrossover.h>
#include <minion/crossover_operators/object/MinionObjectCrossover.h>
#include "MinionCrossover.h"

class CustomMinionCrossover : public MinionCrossover {
    std::vector<std::shared_ptr<MinionControllerCrossover>> controllerOps;
    std::vector<std::shared_ptr<MinionSensesCrossover>> sensesOps;
    std::vector<std::shared_ptr<MinionObjectCrossover>> objectOps;

    std::vector<float> controllerOpsProbs, sensesOpsProbs, objectOpsProbs;

    void addObjectCrossoverOp(rjs::Value &root);
    void addSensesCrossoverOp(rjs::Value &root);
    void addControllerCrossoverOp(rjs::Value &root);
    void fixProbabilities(std::vector<float> &probs);
    int getRandomIndexFromProbs(std::vector<float> &probs);

public:
    CustomMinionCrossover();

    ~CustomMinionCrossover();

    void configureFromJSON(rjs::Value &root) override;

    std::shared_ptr<Minion> crossover(std::shared_ptr<Minion> m1, std::shared_ptr<Minion> m2) override;
};


#endif //BIOME_SIMULATION_CUSTOMCROSSOVER_H

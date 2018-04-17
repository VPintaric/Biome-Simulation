#include <random>
#include <helpers/RNG.h>
#include <minion/crossover_operators/object/FitnessWeightedAverageObjectCrossover.h>
#include <minion/crossover_operators/senses/FitnessWeightedAverageSensesCrossover.h>
#include <minion/crossover_operators/controller/PickBetterControllerCrossover.h>
#include <minion/crossover_operators/senses/PickBetterSensesCrossover.h>
#include <minion/crossover_operators/object/PickBetterObjectCrossover.h>
#include "minion/crossover_operators/CustomMinionCrossover.h"

CustomMinionCrossover::CustomMinionCrossover() = default;

CustomMinionCrossover::~CustomMinionCrossover() = default;

std::shared_ptr<Minion> CustomMinionCrossover::crossover(std::shared_ptr<Minion> m1, std::shared_ptr<Minion> m2) {
    auto child = std::make_shared<Minion>();

    int operatorIdx = getRandomIndexFromProbs(objectOpsProbs);
    auto object = objectOps[operatorIdx]->crossover(m1->getObject(), m2->getObject(),
                                                    m1->getFitness(), m2->getFitness());

    operatorIdx = getRandomIndexFromProbs(sensesOpsProbs);
    auto senses = sensesOps[operatorIdx]->crossover(m1->getSenses(), m2->getSenses(),
                                                    m1->getFitness(), m2->getFitness());

    operatorIdx = getRandomIndexFromProbs(controllerOpsProbs);
    auto controller = controllerOps[operatorIdx]->crossover(m1->getController(), m2->getController(),
                                                            m1->getFitness(), m2->getFitness());

    senses->setMinion(child);

    child->setObject(object);
    child->setSenses(senses);
    child->setController(controller);

    return child;
}

int CustomMinionCrossover::getRandomIndexFromProbs(std::vector<float> &probs) {
    std::uniform_real_distribution<float> uniformDistr(0.f, 1.f);

    float roll = uniformDistr(RNG::get());

    float cumSum = 0.f;

    for(int idx = 0; idx < probs.size(); idx++){
        cumSum += probs[idx];
        if(roll <= cumSum){
            return idx;
        }
    }

    return static_cast<int>(probs.size() - 1);
}

void CustomMinionCrossover::configureFromJSON(rjs::Value &root) {
    const char * OBJECT_CROSSOVER_OPS = "object_crossovers";
    const char * SENSES_CROSSOVER_OPS = "senses_crossovers";
    const char * CONTROLLER_CROSSOVER_OPS = "controller_crossovers";

    if(root.HasMember(OBJECT_CROSSOVER_OPS) && root[OBJECT_CROSSOVER_OPS].IsArray()){
        auto crossovers = root[OBJECT_CROSSOVER_OPS].GetArray();

        for(auto& op : crossovers){
            addObjectCrossoverOp(op);
        }

        fixProbabilities(objectOpsProbs);
    } else {
        objectOps.push_back(std::make_shared<FitnessWeightedAverageObjectCrossover>());
        objectOpsProbs.push_back(1.f);
    }

    if(root.HasMember(SENSES_CROSSOVER_OPS) && root[SENSES_CROSSOVER_OPS].IsArray()){
        auto crossovers = root[SENSES_CROSSOVER_OPS].GetArray();

        for(auto& op : crossovers){
            addSensesCrossoverOp(op);
        }

        fixProbabilities(sensesOpsProbs);
    } else {
        sensesOps.push_back(std::make_shared<FitnessWeightedAverageSensesCrossover>());
        sensesOpsProbs.push_back(1.f);
    }

    if(root.HasMember(CONTROLLER_CROSSOVER_OPS) && root[CONTROLLER_CROSSOVER_OPS].IsArray()){
        auto crossovers = root[CONTROLLER_CROSSOVER_OPS].GetArray();

        for(auto& op : crossovers){
            addControllerCrossoverOp(op);
        }

        fixProbabilities(controllerOpsProbs);
    } else {
        controllerOps.push_back(std::make_shared<PickBetterControllerCrossover>());
        controllerOpsProbs.push_back(1.f);
    }
}

void CustomMinionCrossover::addObjectCrossoverOp(rjs::Value &root) {
    const char * NAME = "name";
    const char * PROBABILITY = "probability";
    const char * CONFIG = "config";

    std::shared_ptr<MinionObjectCrossover> crossoverOp;

    if(root.HasMember(NAME) && root[NAME].IsString()) {
        std::string opName = root[NAME].GetString();

        if(opName == "pick_better"){
            crossoverOp = std::make_shared<PickBetterObjectCrossover>();
        } else if(opName == "fitness_weighted_average"){
            crossoverOp = std::make_shared<FitnessWeightedAverageObjectCrossover>();
        } else {
            crossoverOp = std::make_shared<PickBetterObjectCrossover>();
        }
    } else {
        return;
    }

    if(root.HasMember(PROBABILITY) && root[PROBABILITY].IsFloat()){
        objectOpsProbs.push_back(root[PROBABILITY].GetFloat());
    } else {
        objectOpsProbs.push_back(-1.f);
    }

    if(root.HasMember(CONFIG) && root[CONFIG].IsObject()){
        crossoverOp->configureFromJSON(root[CONFIG]);
    }

    objectOps.push_back(crossoverOp);
}

void CustomMinionCrossover::addSensesCrossoverOp(rjs::Value &root) {
    const char * NAME = "name";
    const char * PROBABILITY = "probability";
    const char * CONFIG = "config";

    std::shared_ptr<MinionSensesCrossover> crossoverOp;

    if(root.HasMember(NAME) && root[NAME].IsString()) {
        std::string opName = root[NAME].GetString();

        if(opName == "fitness_weighted_average"){
            crossoverOp = std::make_shared<FitnessWeightedAverageSensesCrossover>();
        } else if(opName == "pick_better"){
            crossoverOp = std::make_shared<PickBetterSensesCrossover>();
        } else {
            crossoverOp = std::make_shared<PickBetterSensesCrossover>();
        }
    } else {
        return;
    }

    if(root.HasMember(PROBABILITY) && root[PROBABILITY].IsFloat()){
        sensesOpsProbs.push_back(root[PROBABILITY].GetFloat());
    } else {
        sensesOpsProbs.push_back(-1.f);
    }

    if(root.HasMember(CONFIG) && root[CONFIG].IsObject()){
        crossoverOp->configureFromJSON(root[CONFIG]);
    }

    sensesOps.push_back(crossoverOp);
}

void CustomMinionCrossover::addControllerCrossoverOp(rjs::Value &root) {
    const char * NAME = "name";
    const char * PROBABILITY = "probability";
    const char * CONFIG = "config";

    std::shared_ptr<MinionControllerCrossover> crossoverOp;

    if(root.HasMember(NAME) && root[NAME].IsString()) {
        std::string opName = root[NAME].GetString();

        if(opName == "pick_better"){
            crossoverOp = std::make_shared<PickBetterControllerCrossover>();
        } else {
            crossoverOp = std::make_shared<PickBetterControllerCrossover>();
        }
    } else {
        return;
    }

    if(root.HasMember(PROBABILITY) && root[PROBABILITY].IsFloat()){
        controllerOpsProbs.push_back(root[PROBABILITY].GetFloat());
    } else {
        controllerOpsProbs.push_back(-1.f);
    }

    if(root.HasMember(CONFIG) && root[CONFIG].IsObject()){
        crossoverOp->configureFromJSON(root[CONFIG]);
    }

    controllerOps.push_back(crossoverOp);
}

void CustomMinionCrossover::fixProbabilities(std::vector<float> &probs) {
    float leftProb = 1.f;
    int nUnknown = 0;

    for(float val : probs){
        if(val >= 0.f){
            leftProb -= val;
        } else {
            nUnknown++;
        }
    }

    if(nUnknown){
        for(float& val : probs){
            if(val < 0.f){
                val = leftProb / nUnknown;
            }
        }
    }
}
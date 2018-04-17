#include <random>
#include <helpers/RNG.h>
#include <minion/mutation_operators/object/GaussNoiseObjectMutation.h>
#include <minion/mutation_operators/senses/GaussNoiseSensesMutation.h>
#include <minion/mutation_operators/controller/neural_net/NeuralNetGaussNoiseWeightsMutation.h>
#include <minion/mutation_operators/controller/neural_net/NeuralNetSparseResetWeightsMutation.h>
#include "minion/mutation_operators/CustomMinionMutation.h"

void CustomMinionMutation::mutate(std::shared_ptr<Minion> m) {
    int operatorIdx = getRandomIndexFromProbs(objectOpsProbs);
    objectOps[operatorIdx]->mutate(m->getObject());

    operatorIdx = getRandomIndexFromProbs(sensesOpsProbs);
    sensesOps[operatorIdx]->mutate(m->getSenses());

    operatorIdx = getRandomIndexFromProbs(controllerOpsProbs);
    controllerOps[operatorIdx]->mutate(m->getController());
}

int CustomMinionMutation::getRandomIndexFromProbs(std::vector<float> &probs) {
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

void CustomMinionMutation::configureFromJSON(rjs::Value &root) {
    const char * OBJECT_MUTATION_OPS = "object_mutations";
    const char * SENSES_MUTATION_OPS = "senses_mutations";
    const char * CONTROLLER_MUTATION_OPS = "controller_mutations";

    if(root.HasMember(OBJECT_MUTATION_OPS) && root[OBJECT_MUTATION_OPS].IsArray()){
        auto crossovers = root[OBJECT_MUTATION_OPS].GetArray();

        for(auto& op : crossovers){
            addObjectMutationOp(op);
        }

        fixProbabilities(objectOpsProbs);
    } else {
        objectOps.push_back(std::make_shared<GaussNoiseObjectMutation>());
        objectOpsProbs.push_back(1.f);
    }

    if(root.HasMember(SENSES_MUTATION_OPS) && root[SENSES_MUTATION_OPS].IsArray()){
        auto crossovers = root[SENSES_MUTATION_OPS].GetArray();

        for(auto& op : crossovers){
            addSensesMutationOp(op);
        }

        fixProbabilities(sensesOpsProbs);
    } else {
        sensesOps.push_back(std::make_shared<GaussNoiseSensesMutation>());
        sensesOpsProbs.push_back(1.f);
    }

    if(root.HasMember(CONTROLLER_MUTATION_OPS) && root[CONTROLLER_MUTATION_OPS].IsArray()){
        auto crossovers = root[CONTROLLER_MUTATION_OPS].GetArray();

        for(auto& op : crossovers){
            addControllerMutationOp(op);
        }

        fixProbabilities(controllerOpsProbs);
    } else {
        controllerOps.push_back(std::make_shared<NeuralNetGaussNoiseWeightsMutation>());
        controllerOpsProbs.push_back(1.f);
    }
}

void CustomMinionMutation::addObjectMutationOp(rjs::Value &root) {
    const char * NAME = "name";
    const char * PROBABILITY = "probability";
    const char * CONFIG = "config";

    std::shared_ptr<MinionObjectMutation> mutationOp;

    if(root.HasMember(NAME) && root[NAME].IsString()) {
        std::string opName = root[NAME].GetString();

        if(opName == "gauss_noise"){
            mutationOp = std::make_shared<GaussNoiseObjectMutation>();
        } else {
            mutationOp = std::make_shared<GaussNoiseObjectMutation>();
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
        mutationOp->configureFromJSON(root[CONFIG]);
    }

    objectOps.push_back(mutationOp);
}

void CustomMinionMutation::addSensesMutationOp(rjs::Value &root) {
    const char * NAME = "name";
    const char * PROBABILITY = "probability";
    const char * CONFIG = "config";

    std::shared_ptr<MinionSensesMutation> mutationOp;

    if(root.HasMember(NAME) && root[NAME].IsString()) {
        std::string opName = root[NAME].GetString();

        if(opName == "gauss_noise"){
            mutationOp = std::shared_ptr<MinionSensesMutation>(new GaussNoiseSensesMutation);
        } else {
            mutationOp = std::shared_ptr<MinionSensesMutation>(new GaussNoiseSensesMutation);
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
        mutationOp->configureFromJSON(root[CONFIG]);
    }

    sensesOps.push_back(mutationOp);
}

void CustomMinionMutation::addControllerMutationOp(rjs::Value &root) {
    const char * NAME = "name";
    const char * PROBABILITY = "probability";
    const char * CONFIG = "config";

    std::shared_ptr<MinionControllerMutation> mutationOp;

    if(root.HasMember(NAME) && root[NAME].IsString()) {
        std::string opName = root[NAME].GetString();

        if(opName == "gauss_noise_weights") {
            mutationOp = std::make_shared<NeuralNetGaussNoiseWeightsMutation>();
        } else if(opName == "gauss_sparse_reset"){
            mutationOp = std::make_shared<NeuralNetSparseResetWeightsMutation>();
        } else {
            mutationOp = std::make_shared<NeuralNetGaussNoiseWeightsMutation>();
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
        mutationOp->configureFromJSON(root[CONFIG]);
    }

    controllerOps.push_back(mutationOp);
}

void CustomMinionMutation::fixProbabilities(std::vector<float> &probs) {
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
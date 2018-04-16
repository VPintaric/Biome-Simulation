#include <minion/controllers/NeuralNetController.h>
#include <minion/senses/simple/SimpleMinionSenses.h>
#include <state/State.h>
#include <state/Log.h>
#include <minion/factories/neuralnet/crossover/ArithmeticAverageCrossover.h>
#include <minion/factories/neuralnet/crossover/InterleaveLayersCrossover.h>
#include <minion/factories/neuralnet/crossover/PickRandomCrossover.h>
#include "minion/factories/neuralnet/NeuralNetMinionGenerator.h"
#include "minion/factories/neuralnet/mutation/GaussNoiseMutation.h"
#include "minion/factories/neuralnet/mutation/SparseResetMutation.h"

NeuralNetMinionGenerator::NeuralNetMinionGenerator(std::vector<int> nnHiddenLayers)
                                                : nnHiddenLayers(nnHiddenLayers){

};

NeuralNetMinionGenerator::~NeuralNetMinionGenerator() = default;

std::shared_ptr<Minion> NeuralNetMinionGenerator::createRawMinion() {
    auto minion = std::make_shared<Minion>();
    auto object = std::make_shared<MinionObject>();
    minion->setObject(object);
    auto senses = std::make_shared<SimpleMinionSenses>(minion);
    minion->setSenses(senses);
    auto controller = std::shared_ptr<NeuralNetController>(new NeuralNetController(minion, nnHiddenLayers, tanhf));
    minion->setController(controller);

    return minion;
}

std::shared_ptr<Minion> NeuralNetMinionGenerator::generateMinion() {
    std::uniform_real_distribution<float> colorDistr(0.f, 1.f);
    std::uniform_real_distribution<float> radiusDistr(SimConst::MINION_MIN_RADIUS, SimConst::MINION_MAX_RADIUS);
    std::uniform_real_distribution<float> senseDistDistr(SimConst::MINION_MIN_MAX_SENSE_DISTANCE,
                                                           SimConst::MINION_MAX_MAX_SENSE_DISTANCE);

    auto minion = createRawMinion();
    auto object = minion->getObject();
    auto controller = std::static_pointer_cast<NeuralNetController>(minion->getController());
    auto senses = minion->getSenses();

    auto rng = State::getInstance().getRng();

    object->setSkinColor(glm::vec4(colorDistr(rng.get()), colorDistr(rng.get()), colorDistr(rng.get()), 1.f));
    object->setRadius(radiusDistr(rng.get()));

    controller->getNeuralNet()->initRandom();

    senses->setMaxSenseDistance(senseDistDistr(rng.get()));

    return minion;
}

std::shared_ptr<Minion> NeuralNetMinionGenerator::generateChild(std::shared_ptr<Minion> first,
                                                                std::shared_ptr<Minion> second) {
    auto child = crossover(first, second);
    mutate(child);
    return child;
}

std::shared_ptr<Minion> NeuralNetMinionGenerator::crossover(std::shared_ptr<Minion> first,
                                                            std::shared_ptr<Minion> second) {
    auto rng = State::getInstance().getRng();

    std::uniform_real_distribution<float> uniformDistr(0.f, 1.f);
    float coeff = uniformDistr(rng.get());

    auto child = createRawMinion();
    auto object = child->getObject();
    auto controller = std::static_pointer_cast<NeuralNetController>(child->getController());
    auto senses = child->getSenses();
    auto nn = controller->getNeuralNet();

    object->setSkinColor(coeff * first->getObject()->getSkinColor() + (1 - coeff) * second->getObject()->getSkinColor());
    object->setRadius(coeff * first->getObject()->getRadius() + (1 - coeff) * second->getObject()->getRadius());

    senses->setMaxSenseDistance(coeff * first->getSenses()->getMaxSenseDistance() +
                                (1 - coeff) * second->getSenses()->getMaxSenseDistance());

    int operatorIdx = getRandomIndexFromProbs(crossoverOpProbs);
    auto first_nn = std::static_pointer_cast<NeuralNetController>(first->getController())->getNeuralNet();
    auto second_nn = std::static_pointer_cast<NeuralNetController>(second->getController())->getNeuralNet();
    crossoverOps[operatorIdx]->crossover(first_nn, second_nn, nn);

    return child;
}

void NeuralNetMinionGenerator::mutate(std::shared_ptr<Minion> m) {
    std::normal_distribution<float> colorMut(0.f, 0.1f);
    std::normal_distribution<float> radiusMut(0.f, 5.f);
    std::normal_distribution<float> senseDistMut(0.f, 5.f);

    auto rng = State::getInstance().getRng();

    auto object = m->getObject();
    auto controller = std::static_pointer_cast<NeuralNetController>(m->getController());
    auto senses = m->getSenses();
    auto nn = controller->getNeuralNet();

    auto c = object->getSkinColor();
    object->setSkinColor(glm::vec4(c.r + colorMut(rng.get()), c.g + colorMut(rng.get()), c.b + colorMut(rng.get()), 1.f));
    object->setRadius(object->getRadius() + radiusMut(rng.get()));
    senses->setMaxSenseDistance(senses->getMaxSenseDistance() + senseDistMut(rng.get()));

    int operatorIdx = getRandomIndexFromProbs(mutationOpProbs);
    mutationOps[operatorIdx]->mutate(nn);
}

void NeuralNetMinionGenerator::configureFromJSON(rjs::Value &root) {
    const char * ARCHITECTURE = "architecture_hidden";
    const char * MUTATION_OPS = "mutations";
    const char * CROSSOVER_OPS = "crossovers";

    if(root.HasMember(ARCHITECTURE) && root[ARCHITECTURE].IsArray()){
        auto array = root[ARCHITECTURE].GetArray();

        nnHiddenLayers.clear();
        for(const auto &x : array){
            if(x.IsInt()){
                nnHiddenLayers.push_back(x.GetInt());
            } else {
                Log().Get(logWARNING) << "NN architecture configuration: element in architecture array is not an int, ignoring...";
            }
        }
    } else {
        nnHiddenLayers = { 40, 30 };
    }

    if(root.HasMember(MUTATION_OPS) && root[MUTATION_OPS].IsArray()){
        auto mutations = root[MUTATION_OPS].GetArray();

        for(auto& op : mutations){
            configAddMutationOp(op);
        }

        fixProbabilities(mutationOpProbs);
    } else {
        mutationOps.push_back(std::make_shared<GaussNoiseMutation>());
        mutationOpProbs.push_back(1.f);
    }

    if(root.HasMember(CROSSOVER_OPS) && root[CROSSOVER_OPS].IsArray()){
        auto crossovers = root[CROSSOVER_OPS].GetArray();

        for(auto& op : crossovers){
            configAddCrossoverOp(op);
        }

        fixProbabilities(crossoverOpProbs);
    } else {
        crossoverOps.push_back(std::make_shared<ArithmeticAverageCrossover>());
        crossoverOpProbs.push_back(1.f);
    }
}

void NeuralNetMinionGenerator::configAddMutationOp(rjs::Value &root) {
    const char * NAME = "name";
    const char * PROBABILITY = "probability";
    const char * CONFIG = "config";

    std::shared_ptr<NeuralNetMutation> mutationOp;

    if(root.HasMember(NAME) && root[NAME].IsString()){
        std::string opName = root[NAME].GetString();

        if(opName == "gauss_noise"){
            mutationOp = std::make_shared<GaussNoiseMutation>();
        } else if(opName == "sparse_reset"){
            mutationOp = std::make_shared<SparseResetMutation>();
        } else {
            mutationOp = std::make_shared<GaussNoiseMutation>();
        }
    } else {
        return;
    }

    if(root.HasMember(PROBABILITY) && root[PROBABILITY].IsFloat()){
        mutationOpProbs.push_back(root[PROBABILITY].GetFloat());
    } else {
        mutationOpProbs.push_back(-1.f);
    }

    if(root.HasMember(CONFIG) && root[CONFIG].IsObject()){
        mutationOp->configureFromJSON(root[CONFIG]);
    }

    mutationOps.push_back(mutationOp);
}

void NeuralNetMinionGenerator::configAddCrossoverOp(rjs::Value &root) {
    const char * NAME = "name";
    const char * PROBABILITY = "probability";
    const char * CONFIG = "config";

    std::shared_ptr<NeuralNetCrossover> crossoverOp;

    if(root.HasMember(NAME) && root[NAME].IsString()) {
        std::string opName = root[NAME].GetString();

        if (opName == "arithmetic_average") {
            crossoverOp = std::make_shared<ArithmeticAverageCrossover>();
        } else if(opName == "interleave_layers") {
            crossoverOp = std::make_shared<InterleaveLayersCrossover>();
        } else if(opName == "pick_random"){
            crossoverOp = std::make_shared<PickRandomCrossover>();
        } else {
            crossoverOp = std::make_shared<ArithmeticAverageCrossover>();
        }
    } else {
        return;
    }

    if(root.HasMember(PROBABILITY) && root[PROBABILITY].IsFloat()){
        crossoverOpProbs.push_back(root[PROBABILITY].GetFloat());
    } else {
        crossoverOpProbs.push_back(-1.f);
    }

    if(root.HasMember(CONFIG) && root[CONFIG].IsObject()){
        crossoverOp->configureFromJSON(root[CONFIG]);
    }

    crossoverOps.push_back(crossoverOp);
}

void NeuralNetMinionGenerator::fixProbabilities(std::vector<float> &v) {
    float leftProb = 1.f;
    int nUnknown = 0;

    for(float val : v){
        if(val >= 0.f){
            leftProb -= val;
        } else {
            nUnknown++;
        }
    }

    if(nUnknown){
        for(float& val : v){
            if(val < 0.f){
                val = leftProb / nUnknown;
            }
        }
    }
}

int NeuralNetMinionGenerator::getRandomIndexFromProbs(std::vector<float> &probs) {
    auto rng = State::getInstance().getRng();

    std::uniform_real_distribution<float> uniformDistr(0.f, 1.f);

    float roll = uniformDistr(rng.get());

    float cumSum = 0.f;

    for(int idx = 0; idx < probs.size(); idx++){
        cumSum += probs[idx];
        if(roll <= cumSum){
            return idx;
        }
    }

    return static_cast<int>(probs.size() - 1);
}

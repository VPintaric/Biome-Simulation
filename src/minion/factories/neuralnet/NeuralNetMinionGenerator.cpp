#include <minion/controllers/NeuralNetController.h>
#include <minion/senses/simple/SimpleMinionSenses.h>
#include <state/State.h>
#include <state/Log.h>
#include <minion/factories/neuralnet/crossover/ArithmeticAverageCrossover.h>
#include <minion/factories/neuralnet/crossover/InterleaveLayersCrossover.h>
#include <minion/factories/neuralnet/crossover/PickRandomCrossover.h>
#include <helpers/RNG.h>
#include "minion/factories/neuralnet/NeuralNetMinionGenerator.h"
#include "minion/factories/neuralnet/mutation/GaussNoiseMutation.h"
#include "minion/factories/neuralnet/mutation/SparseResetMutation.h"

NeuralNetMinionGenerator::NeuralNetMinionGenerator(std::vector<int> nnHiddenLayers)
                                                : nnHiddenLayers(nnHiddenLayers){

};

NeuralNetMinionGenerator::~NeuralNetMinionGenerator() = default;

std::shared_ptr<Minion> NeuralNetMinionGenerator::generateRandomMinion() {
    std::uniform_real_distribution<float> colorDistr(0.f, 1.f);
    std::uniform_real_distribution<float> radiusDistr(SimConst::MINION_MIN_RADIUS, SimConst::MINION_MAX_RADIUS);
    std::uniform_real_distribution<float> senseDistDistr(SimConst::MINION_MIN_MAX_SENSE_DISTANCE,
                                                           SimConst::MINION_MAX_MAX_SENSE_DISTANCE);

    auto minion = generateMinion();
    auto object = minion->getObject();
    auto controller = std::static_pointer_cast<NeuralNetController>(minion->getController());
    auto senses = minion->getSenses();

    object->setSkinColor(glm::vec4(colorDistr(RNG::get()), colorDistr(RNG::get()), colorDistr(RNG::get()), 1.f));
    object->setRadius(radiusDistr(RNG::get()));

    controller->getNeuralNet()->initRandom();

    senses->setMaxSenseDistance(senseDistDistr(RNG::get()));

    return minion;
}

void NeuralNetMinionGenerator::configureFromJSON(rjs::Value &root) {
    const char * ARCHITECTURE = "architecture_hidden";

    Log().Get(logDEBUG) << "Configuring NeuralNetMinionGenerator object...";

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

    Log().Get(logDEBUG) << "Neural net architecture hidden layers set to: ";
    std::stringstream ss;
    for(auto x : nnHiddenLayers){
        ss << x << " ";
    }
    Log().Get(logDEBUG) << ss.str();

    Log().Get(logDEBUG) << "Finished configuring NeuralNetMinionGenerator object";
}

std::shared_ptr<Minion> NeuralNetMinionGenerator::generateMinion() {
    auto minion = std::make_shared<Minion>();
    auto object = std::make_shared<MinionObject>();
    minion->setObject(object);
    auto senses = std::make_shared<SimpleMinionSenses>(minion);
    minion->setSenses(senses);
    auto controller = std::shared_ptr<NeuralNetController>(new NeuralNetController(senses->getDataSize(), nnHiddenLayers, tanhf));
    minion->setController(controller);

    return minion;
}

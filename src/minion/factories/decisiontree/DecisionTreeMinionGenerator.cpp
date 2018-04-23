#include <random>
#include <constants/SimulationConstants.h>
#include <minion/controllers/decision_tree/DecisionTreeController.h>
#include <helpers/RNG.h>
#include <minion/senses/simple/SimpleMinionSenses.h>
#include "minion/factories/decisiontree/DecisionTreeMinionGenerator.h"

DecisionTreeMinionGenerator::DecisionTreeMinionGenerator() : maxTreeDepth(50) {

}

void DecisionTreeMinionGenerator::configureFromJSON(rjs::Value &root) {
    const char * MAX_TREE_DEPTH = "max_tree_depth";

    if(root.HasMember(MAX_TREE_DEPTH) && root[MAX_TREE_DEPTH].IsInt()){
        maxTreeDepth = root[MAX_TREE_DEPTH].GetInt();
    }
}

std::shared_ptr<Minion> DecisionTreeMinionGenerator::generateRandomMinion() {
    std::uniform_real_distribution<float> colorDistr(0.f, 1.f);
    std::uniform_real_distribution<float> radiusDistr(SimConst::MINION_MIN_RADIUS, SimConst::MINION_MAX_RADIUS);
    std::uniform_real_distribution<float> senseDistDistr(SimConst::MINION_MIN_MAX_SENSE_DISTANCE,
                                                         SimConst::MINION_MAX_MAX_SENSE_DISTANCE);

    auto minion = generateMinion();
    auto object = minion->getObject();
    auto controller = std::static_pointer_cast<DecisionTreeController>(minion->getController());
    auto senses = minion->getSenses();

    object->setSkinColor(glm::vec4(colorDistr(RNG::get()), colorDistr(RNG::get()), colorDistr(RNG::get()), 1.f));
    object->setRadius(radiusDistr(RNG::get()));

    controller->initRandom();

    senses->setMaxSenseDistance(senseDistDistr(RNG::get()));

    return minion;
}

std::shared_ptr<Minion> DecisionTreeMinionGenerator::generateMinion() {
    auto minion = std::make_shared<Minion>();
    auto object = std::make_shared<MinionObject>();
    minion->setObject(object);

    auto senses = std::make_shared<SimpleMinionSenses>(minion);
    senses->setMinion(minion);
    minion->setSenses(senses);

    auto controller = std::make_shared<DecisionTreeController>(maxTreeDepth);
    controller->setMinion(minion);
    minion->setController(controller);

    return minion;
}

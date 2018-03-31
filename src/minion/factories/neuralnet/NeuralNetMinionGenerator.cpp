#include <minion/controllers/NeuralNetController.h>
#include <minion/senses/simple/SimpleMinionSenses.h>
#include "minion/factories/neuralnet/NeuralNetMinionGenerator.h"

NeuralNetMinionGenerator::NeuralNetMinionGenerator() = default;
NeuralNetMinionGenerator::~NeuralNetMinionGenerator() = default;

std::shared_ptr<Minion> NeuralNetMinionGenerator::generateMinion() {
    auto minion = std::make_shared<Minion>();
    auto object = std::make_shared<MinionObject>();
    auto senses = std::make_shared<SimpleMinionSenses>(minion);
    auto controller = std::shared_ptr<NeuralNetController>(new NeuralNetController({10, 10}, tanhf));

    minion->setObject(object);
    minion->setSenses(senses);
    minion->setController(controller);

    object->setSkinColor(glm::vec4(0.f, 0.f, 1.f, 1.f));
    object->setRadius(15.f);
    object->setRMass(20.f / (object->getRadius()));

    controller->getNeuralNet()->initRandom();

    senses->setMaxSenseDistance(100.f);

    return minion;
}

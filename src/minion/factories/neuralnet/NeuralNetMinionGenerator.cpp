#include <minion/controllers/NeuralNetController.h>
#include <minion/senses/simple/SimpleMinionSenses.h>
#include "minion/factories/neuralnet/NeuralNetMinionGenerator.h"

NeuralNetMinionGenerator::NeuralNetMinionGenerator() = default;
NeuralNetMinionGenerator::~NeuralNetMinionGenerator() = default;

std::shared_ptr<Minion> NeuralNetMinionGenerator::generateMinion() {
    auto minion = std::make_shared<Minion>();
    auto object = std::make_shared<MinionObject>();
    auto senses = std::make_shared<SimpleMinionSenses>(minion);
    auto controller = std::shared_ptr<NeuralNetController>(new NeuralNetController(minion, {10, 10}, tanhf));

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

std::shared_ptr<Minion> NeuralNetMinionGenerator::generateChild(std::shared_ptr<Minion> first,
                                                                std::shared_ptr<Minion> second) {
    auto child = crossover(first, second);
    mutate(child);
    return child;
}

std::shared_ptr<Minion> NeuralNetMinionGenerator::crossover(std::shared_ptr<Minion> first,
                                                            std::shared_ptr<Minion> second) {
    return nullptr;
}

void NeuralNetMinionGenerator::mutate(std::shared_ptr<Minion> m) {

}
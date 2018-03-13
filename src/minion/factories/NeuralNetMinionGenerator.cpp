#include <minion/controllers/NeuralNetController.h>
#include "minion/factories/NeuralNetMinionGenerator.h"

NeuralNetMinionGenerator::NeuralNetMinionGenerator() = default;
NeuralNetMinionGenerator::~NeuralNetMinionGenerator() = default;

std::shared_ptr<Minion> NeuralNetMinionGenerator::generateMinion() {
    auto minion = std::make_shared<Minion>();
    auto object = std::make_shared<MinionObject>();
    auto senses = std::make_shared<MinionSenses>();
    auto controller = std::shared_ptr<NeuralNetController>(new NeuralNetController({10, 10}, tanhf));

    object->setSkinColor(glm::vec4(0.f, 0.f, 1.f, 1.f));
    object->setRadius(15.f);
    object->setRMass(20.f / (object->getRadius()));
    object->setDecay(2.5f);
    object->setMaxLife(100.f);
    object->setMinLife(-100.f);
    object->setLife(100.f);

    minion->setObject(object);
    minion->setSenses(senses);
    minion->setController(controller);

    return minion;
}

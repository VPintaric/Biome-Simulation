#include <minion/senses/simple/SimpleMinionSenses.h>
#include <minion/controllers/hardcoded/HardcodedController.h>
#include "minion/factories/hardcoded/HardcodedMinionGenerator.h"

void HardcodedMinionGenerator::configureFromJSON(rjs::Value &root) {
    // Nothing to configure
}

std::shared_ptr<Minion> HardcodedMinionGenerator::generateRandomMinion() {
    return generateMinion();
}

std::shared_ptr<Minion> HardcodedMinionGenerator::generateMinion() {
    auto m = std::make_shared<Minion>();

    auto object = std::make_shared<MinionObject>();
    auto senses = std::make_shared<SimpleMinionSenses>(m);
    auto controller = std::make_shared<HardcodedController>();

    m->setObject(object);
    m->setSenses(senses);
    m->setController(controller);

    object->setRadius(20.f);
    object->setSkinColor(glm::vec4(1.f, 1.f, 0.f, 1.f));
    senses->setMaxSenseDistance(350.f);

    senses->setMinion(m);
    controller->setMinion(m);

    return m;
}

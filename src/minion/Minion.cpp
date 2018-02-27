#include "Minion.h"

Minion::Minion() = default;
Minion::~Minion() = default;

void Minion::update(float deltaT) {
    controller->controlMinion(object, senses);
    senses->clearData();
    object->update(deltaT);
}

void Minion::draw() {
    object->draw();
}

const std::shared_ptr<MinionObject> &Minion::getObject() const {
    return object;
}

void Minion::setObject(const std::shared_ptr<MinionObject> &object) {
    Minion::object = object;
}

const std::shared_ptr<MinionSenses> &Minion::getSenses() const {
    return senses;
}

void Minion::setSenses(const std::shared_ptr<MinionSenses> &senses) {
    Minion::senses = senses;
}

const std::shared_ptr<MinionController> &Minion::getController() const {
    return controller;
}

void Minion::setController(const std::shared_ptr<MinionController> &controller) {
    Minion::controller = controller;
}

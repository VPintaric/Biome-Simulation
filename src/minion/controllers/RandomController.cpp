#include "RandomController.h"
#include "../../state/State.h"

RandomController::RandomController() = default;
RandomController::~RandomController() = default;

void RandomController::controlMinion(const std::shared_ptr<MinionObject> &m, const std::shared_ptr<MinionSenses> &senses) {
    auto rng = State::getInstance().getRng();
    std::normal_distribution<float> rotMom(0.f, 1e8f);
    std::normal_distribution<float> force(100.f, 1e5f);

    m->setControlForce(force(rng.get()));
    m->setControlRotMoment(rotMom(rng.get()));
}
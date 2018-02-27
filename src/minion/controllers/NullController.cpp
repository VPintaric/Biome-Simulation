#include "minion/controllers/NullController.h"

NullController::NullController() = default;
NullController::~NullController() = default;

void NullController::controlMinion(const std::shared_ptr<MinionObject> &m, const std::shared_ptr<MinionSenses> &senses) {
    m->setControlRotMoment(0.f);
    m->setControlForce(0.f);
}

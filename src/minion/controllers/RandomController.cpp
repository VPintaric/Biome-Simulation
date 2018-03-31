#include "minion/controllers/RandomController.h"
#include "state/State.h"

RandomController::RandomController() = default;
RandomController::~RandomController() = default;

std::vector<float> RandomController::controlMinion(std::vector<float> senseData) {
    auto rng = State::getInstance().getRng();
    std::normal_distribution<float> rotMom(0.f, 1e3f);
    std::normal_distribution<float> force(100.f, 1e5f);

    std::vector<float> controlParams;
    controlParams.push_back(force(rng.get()));
    controlParams.push_back(rotMom(rng.get()));

    return controlParams;
}

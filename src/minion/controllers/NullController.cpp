#include "minion/controllers/NullController.h"

NullController::NullController() = default;
NullController::~NullController() = default;

std::vector<float> NullController::controlMinion(std::vector<float> senseData) {
    std::vector<float> v {5.f, 5.f};
    return v;
}

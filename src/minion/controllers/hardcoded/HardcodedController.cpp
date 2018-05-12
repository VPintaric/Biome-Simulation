#include <minion/senses/simple/SimpleMinionSenses.h>
#include <state/Log.h>
#include <glm/gtc/constants.hpp>
#include <helpers/MathHelpers.h>
#include "minion/controllers/hardcoded/HardcodedController.h"

std::shared_ptr<MinionController> HardcodedController::copy() {
    return std::make_shared<HardcodedController>();
}

std::vector<float> HardcodedController::controlMinion(std::vector<float> senseData) {
    const float MAX_CONTROL_ACC = 500.f;
    const float MAX_CONTROL_ROT = 500.f;

    auto senses = std::dynamic_pointer_cast<SimpleMinionSenses>(lockMinion()->getSenses());

    if(senses == nullptr){
        Log().Get(logWARNING) << "Senses are not an instance of SimpleMinionSenses, can't control minion";
        return std::vector<float>(2, 0);
    }

    auto nSightLines = static_cast<int>((senseData.size() - 5) / 2);
    auto angle = glm::half_pi<float>();
    auto angleDelta = glm::pi<float>() / (nSightLines - 1);
    auto maxDistance = senses->getMaxSenseDistance();

    auto distBehind = senseData[senseData.size() - 5];
    auto life = senseData[senseData.size() - 4];
    auto velocity = senseData[senseData.size() - 3];
    auto velocityAngle = senseData[senseData.size() - 2];
    auto rotVelocity = senseData[senseData.size() - 1];

    float closestDist = maxDistance;
    float closestAngle = 0.f;
    float closestType = 0.f;
    for(int i = 0; i < nSightLines * 2; i += 2){
        float dist = senseData[i];
        float type = senseData[i + 1];

        if(dist < closestDist){
            closestType = type;
            closestAngle = angle;
            closestDist = dist;
        }

        angle -= angleDelta;
    }

    float controlAcc = 0.f, controlRot = 0.f;

    bool controlSet = true;
    if(Math::approx(closestType, 0.f)){
        controlSet = false;
    } else if(Math::approx(closestType, 10.f) || Math::approx(closestType, 30.f) ||
            (Math::approx(closestType, 20.f) && life > 50.f)){
        controlRot = MAX_CONTROL_ROT * closestAngle / glm::half_pi<float>();
        controlAcc = MAX_CONTROL_ACC * (glm::half_pi<float>() - std::abs(closestAngle)) / glm::half_pi<float>();
    } else if(Math::approx(closestType, 40.f) || Math::approx(closestType, 50.f) ||
            (Math::approx(closestType, 20.f) && life <= 50.f)){
        controlRot = (closestAngle <= 0 ? MAX_CONTROL_ROT : -MAX_CONTROL_ROT) *
                (glm::half_pi<float>() - std::abs(closestAngle)) / glm::half_pi<float>();
        controlAcc = closestAngle < -glm::quarter_pi<float>() || closestAngle > glm::quarter_pi<float>() ?
                     MAX_CONTROL_ACC / 3.f
                     :
                     -MAX_CONTROL_ACC * (maxDistance - closestDist) / maxDistance;
    }

    if(!controlSet && distBehind < 0.75f * maxDistance){
        controlSet = true;

        controlRot = -MAX_CONTROL_ROT * rotVelocity;
        controlAcc = MAX_CONTROL_ACC * (maxDistance - distBehind) / maxDistance;
    }

    if(!controlSet){
        controlAcc = MAX_CONTROL_ACC / 5.f;
        controlRot = -MAX_CONTROL_ROT * rotVelocity;
    }

    std::vector<float> control = {controlAcc, controlRot};
    return control;
}

void HardcodedController::setMinion(std::shared_ptr<Minion> m) {
    this->m = m;
}

std::shared_ptr<Minion> HardcodedController::lockMinion() {
    auto lock = m.lock();
    if(lock == nullptr){
        Log().Get(logWARNING) << "Could not get a lock on minion pointer";
    }
    return lock;
}

#include "glm/gtc/constants.hpp"
#include "minion/controllers/CowardController.h"
#include "state/Log.h"
#include "helpers/MathHelpers.h"

CowardController::CowardController() = default;
CowardController::~CowardController() = default;

//void CowardController::controlMinion(const std::shared_ptr<MinionObject> &m, const std::shared_ptr<MinionSenses> &senses) {
//    const float CONST_FORCE = 2000.f;
//    const float CONST_ROT_MOM = 5e3f;
//    const float SAFE_DIVISION_CONST = 1e-1;
//
//    float force = 0.f, rotMom = 0.f;
//
//    auto sense = senses->popSenseData();
//    while(sense != nullptr){
//        float angleSign = Math::sign(sense->angle);
//        if(angleSign == 0.f){
//            angleSign = 1.f;
//        }
//        rotMom += angleSign * (CONST_ROT_MOM / (sense->dist + SAFE_DIVISION_CONST));
//
//        if(sense->angle < glm::quarter_pi<float>() && sense->angle > -glm::quarter_pi<float>()){
//            force -= (CONST_FORCE / (sense->dist + SAFE_DIVISION_CONST));
//        } else if(sense->angle > 3.f * glm::quarter_pi<float>() || sense->angle < -3.f * glm::quarter_pi<float>()){
//            force += (CONST_FORCE / (sense->dist + SAFE_DIVISION_CONST));
//        }
//
//        sense = senses->popSenseData();
//    }
//
//    m->setControlForce(force);
//    m->setControlRotMoment(rotMom);
//}

std::vector<float> CowardController::controlMinion(std::vector<float> senseData) {
    return std::vector<float>();
}

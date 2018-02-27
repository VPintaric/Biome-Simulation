#include <GL/glm/gtc/constants.hpp>
#include "BraveController.h"
#include "../../helpers/MathHelpers.h"
#include "../../state/Log.h"

BraveController::BraveController() = default;
BraveController::~BraveController() = default;

void BraveController::controlMinion(const std::shared_ptr<MinionObject> &m, const std::shared_ptr<MinionSenses> &senses) {
    const float CONST_FORCE = 10.f;
    const float CONST_ROT_MOM = 5e3f;
    const float SAFE_DIVISION_CONST = 1e-1;

    float rotMom = 0.f, force = 0.f;

    auto closest = senses->popSenseData();
    auto sense = senses->popSenseData();
    if(closest != nullptr){
        while(sense != nullptr){
            if(sense->dist < closest->dist){
                closest = sense;
            }
            sense = senses->popSenseData();
        }
    }

    if(closest != nullptr){
        float angleSign = Math::sign(closest->angle);
        rotMom = -angleSign * (CONST_ROT_MOM / (closest->dist + SAFE_DIVISION_CONST));

        force = 0.f;
        if(closest->angle < glm::quarter_pi<float>() && closest->angle > -glm::quarter_pi<float>()){
            force = 200.f * (CONST_FORCE / (closest->dist + SAFE_DIVISION_CONST));
        } else if(closest->angle > 3.f * glm::quarter_pi<float>() || closest->angle < -3.f * glm::quarter_pi<float>()){
            force = -200.f * (CONST_FORCE / (closest->dist + SAFE_DIVISION_CONST));
        }
    } else {
        force = 50.f;
    }

    m->setControlForce(force);
    m->setControlRotMoment(rotMom);
}

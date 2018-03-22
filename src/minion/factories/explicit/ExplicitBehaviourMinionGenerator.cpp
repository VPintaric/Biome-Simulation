#include "glm/gtc/constants.hpp"
#include "minion/factories/explicit/ExplicitBehaviourMinionGenerator.h"
#include "minion/controllers/RandomController.h"
#include "minion/controllers/CowardController.h"
#include "minion/controllers/BraveController.h"
#include "rendering/Renderer.h"
#include "state/State.h"
#include "minion/controllers/NullController.h"

ExplicitBehaviourMinionGenerator::ExplicitBehaviourMinionGenerator() :
        radiusDistr(15.f, 2.f),
        typeDistr(0.f, 1.f)
{

}

ExplicitBehaviourMinionGenerator::~ExplicitBehaviourMinionGenerator() = default;

std::shared_ptr<Minion> ExplicitBehaviourMinionGenerator::generateMinion() {
    auto rng = State::getInstance().getRng();

    auto minion = std::make_shared<Minion>();
    int type = getRandomType();

    auto object = std::make_shared<MinionObject>();
    object->setSkinColor(COLOR_TYPES[type]);
    object->setRadius(std::fabs(radiusDistr(rng.get())) + 1e-1f);
    object->setRMass(20.f / (object->getRadius()));

    auto senses = std::make_shared<MinionSenses>();

    auto controller = getControllerForType(type);

    minion->setObject(object);
    minion->setSenses(senses);
    minion->setController(controller);

    return minion;
}

std::shared_ptr<MinionController> ExplicitBehaviourMinionGenerator::getControllerForType(int type) {
    switch(type){
        case 0:
            return std::make_shared<RandomController>();
        case 1:
            return std::make_shared<CowardController>();
        case 2:
            return std::make_shared<BraveController>();
        case 3:
            return std::make_shared<NullController>();
        default:
            return std::make_shared<RandomController>();
    }
}

int ExplicitBehaviourMinionGenerator::getRandomType() {
    float p = typeDistr(State::getInstance().getRng().get());
    float acc = 0.f;

    for(int type = 0; type < NUM_TYPES; type++){
        if(p <= acc + TYPE_PROBS[type]){
            return type;
        }
        acc += TYPE_PROBS[type];
    }

    return NUM_TYPES - 1;
}
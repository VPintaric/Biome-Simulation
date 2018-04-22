#include <state/Log.h>
#include <minion/senses/simple/SimpleMinionSenses.h>
#include <helpers/MathHelpers.h>
#include "minion/controllers/decision_tree/DecisionTreeController.h"
#include <glm/gtc/constants.hpp>

std::shared_ptr<Minion> DecisionTreeController::lockMinion() {
    auto locked = m.lock();
    if(locked == nullptr){
        Log().Get(logERROR) << "Can't get a lock on minion pointer";
    }
    return locked;
}

std::shared_ptr<MinionController> DecisionTreeController::copy() {
    return nullptr;
}

std::vector<float> DecisionTreeController::controlMinion(std::vector<float> senseData) {
    std::set<int> facts = preprocess(senseData);

    auto accRes = static_cast<DTConst::AccType>(accDecTree->evaluate(facts));
    auto rotRes = static_cast<DTConst::RotType>(rotDecTree->evaluate(facts));

    std::vector<float> res {
        DTConst::accTypeToVal.find(accRes)->second,
        DTConst::rotTypeToVal.find(rotRes)->second
    };

    return res;
}

std::set<int> DecisionTreeController::preprocess(std::vector<float> &senseData) {
    std::set<int> facts;

    auto minion = lockMinion();
    auto senses = std::dynamic_pointer_cast<SimpleMinionSenses>(minion->getSenses());

    if(senses == nullptr){
        Log().Get(logWARNING) << "DecisionTreeController can only preprocess data from SimpleMinionSenses!";
        return facts;
    }

    auto nSightLines = static_cast<int>((senseData.size() - 5) / 5);

    int leftSightLines, frontSightLines;
    leftSightLines = frontSightLines = nSightLines / 3;
    frontSightLines += nSightLines % 3;

    int sightLine = 0;
    while(sightLine < leftSightLines){
        auto detectedType = senseData[sightLine * 5 + 1];
        if(Math::approx(detectedType, 50.f)){
            facts.insert(DTConst::FACT_BODY_LEFT);
        } else if(Math::approx(detectedType, 100.f)) {
            facts.insert(DTConst::FACT_MINION_LEFT);
        } else if(Math::approx(detectedType, 150.f)) {
            facts.insert(DTConst::FACT_PELLET_LEFT);
        } else if(Math::approx(detectedType, 200.f)) {
            facts.insert(DTConst::FACT_BOUNDARY_LEFT);
        }
        ++sightLine;
    }

    while(sightLine < leftSightLines + frontSightLines){
        auto detectedType = senseData[sightLine * 5 + 1];
        if(Math::approx(detectedType, 50.f)){
            facts.insert(DTConst::FACT_BODY_FRONT);
        } else if(Math::approx(detectedType, 100.f)) {
            facts.insert(DTConst::FACT_MINION_FRONT);
        } else if(Math::approx(detectedType, 150.f)) {
            facts.insert(DTConst::FACT_PELLET_FRONT);
        } else if(Math::approx(detectedType, 200.f)) {
            facts.insert(DTConst::FACT_BOUNDARY_FRONT);
        }
        ++sightLine;
    }

    while(sightLine < nSightLines){
        auto detectedType = senseData[sightLine * 5 + 1];
        if(Math::approx(detectedType, 50.f)){
            facts.insert(DTConst::FACT_BODY_RIGHT);
        } else if(Math::approx(detectedType, 100.f)) {
            facts.insert(DTConst::FACT_MINION_RIGHT);
        } else if(Math::approx(detectedType, 150.f)) {
            facts.insert(DTConst::FACT_PELLET_RIGHT);
        } else if(Math::approx(detectedType, 200.f)) {
            facts.insert(DTConst::FACT_BOUNDARY_RIGHT);
        }
        ++sightLine;
    }

    int attrIdx = sightLine * 5;
    if(senseData[attrIdx] < minion->getSenses()->getRadius() - minion->getObject()->getRadius()){
        facts.insert(DTConst::FACT_SOMETHING_BEHIND);
    }
    ++attrIdx;

    if(senseData[attrIdx] > 75.f){
        facts.insert(DTConst::FACT_HEALTHY);
    } else if(minion->getLife() < 25.f){
        facts.insert(DTConst::FACT_UNHEALTHY);
    }
    ++attrIdx;

    if(senseData[attrIdx] >= 60.f){
        facts.insert(DTConst::FACT_MOVING_FAST);
    } else if(senseData[attrIdx] <= 20.f){
        facts.insert(DTConst::FACT_MOVING_SLOW);
    }
    ++attrIdx;

    if(senseData[attrIdx] >= -glm::quarter_pi<float>() && senseData[attrIdx] <= glm::quarter_pi<float>()){
        facts.insert(DTConst::FACT_FRONT_ALIGNED);
    } else if(senseData[attrIdx] <= -3.f * glm::quarter_pi<float>() && senseData[attrIdx] >= 3.f * glm::quarter_pi<float>()){
        facts.insert(DTConst::FACT_FRONT_MISALIGNED);
    }
    ++attrIdx;

    if(senseData[attrIdx] < 0.f){
        facts.insert(DTConst::FACT_ROTATING_COUNTERCLOCKWISE);
    } else if(senseData[attrIdx] > 0.f){
        facts.insert(DTConst::FACT_ROTATING_CLOCKWISE);
    }

    return facts;
}

void DecisionTreeController::setMinion(std::shared_ptr<Minion> m) {
    this->m = m;
}

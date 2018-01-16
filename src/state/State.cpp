#include <random>
#include <chrono>
#include <GL/glm/gtc/constants.hpp>
#include "State.h"
#include "Log.h"
#include "../rendering/Renderer.h"
#include "../constants/SimulationConstants.h"
#include "../constants/WindowConstants.h"
#include "../collision/CollisionInfo.h"
#include "../collision/CollisionDetection.h"
#include "../collision/CollisionResponse.h"

State& State::getInstance() {
    static State instance;
    return instance;
 }

State::State() {
    Log().Get(logDEBUG) << "Creating new state instance";
    shouldEndProgramFlag = false;
    Log().Get(logDEBUG) << "Created new state instance";
}

State::~State() {
    Log().Get(logDEBUG) << "Destroying state instance";
}

void State::endProgram(){
    this->shouldEndProgramFlag = true;
}

bool State::getShouldEndProgram() const {
    return this->shouldEndProgramFlag;
}

void State::spawnMinions(int n) {
    std::uniform_real_distribution<float> posDistr((float) -WindowConst::WINDOW_HEIGHT / 2., (float) WindowConst::WINDOW_HEIGHT / 2.);
    std::uniform_real_distribution<float> colDistr(0.2f, 1.f);
    std::uniform_real_distribution<float> angleDistr(0.f, glm::pi<float>());
    std::normal_distribution<float> radiusDistr(20.f, 10.f);
    std::normal_distribution<float> massDistr(3.f, 1.f);
    nMinions = n;

    Renderer &r = Renderer::getInstance();

//    std::shared_ptr<Minion> minion = std::make_shared<Minion>();
//    minion->setPos(glm::vec2(200.f, 0.f));
//    minion->setColor(glm::vec4(colDistr(rng), colDistr(rng), colDistr(rng), 1.f));
//    minion->setRadius(20.f);
//    minion->setRMass(0.1f);
//    minion->setVelocity(glm::vec2(-50.f, 10.f));
//    minion->setAngleVel(1.f);
//    minion->setAngleAcc(0.f);
//    minion->setAngle(0.f);
//    minions.push_back(minion);
//
//    minion = std::make_shared<Minion>();
//    minion->setPos(glm::vec2(-200.f, 0.f));
//    minion->setColor(glm::vec4(colDistr(rng), colDistr(rng), colDistr(rng), 1.f));
//    minion->setRadius(20.f);
//    minion->setRMass(0.1f);
//    minion->setVelocity(glm::vec2(50.f, 10.f));
//    minion->setAngleVel(1.f);
//    minion->setAngleAcc(0.f);
//    minion->setAngle(0.f);
//    minions.push_back(minion);
    for(int i = 0; i < n; i++){
        std::shared_ptr<Minion> minion = std::make_shared<Minion>();
        minion->setRadius(10.f);
        minion->setPos(glm::vec2(posDistr(rng), posDistr(rng)));
        minion->setColor(glm::vec4(colDistr(rng), colDistr(rng), colDistr(rng), 1.f));
        minion->setAngle(angleDistr(rng));
        minion->setRadius(radiusDistr(rng));
        minion->setRMass(massDistr(rng));
        minion->setAngleVel(0.f);
        minion->setAngleAcc(0.f);
        minion->setRMass(1.f);
        minions.push_back(minion);
    }
}

void State::draw() {
    for(const std::shared_ptr<Minion> &m : minions){
        m->draw();
    }
}

void State::update(float dt) {
    CollisionDetection cd = CollisionDetection::getInstance();
    CollisionResponse cr = CollisionResponse::getInstance();

    std::normal_distribution<float> distrAngleAcc(0.f, 1.f);
    std::normal_distribution<float> distrAcc(0.f, 20.f);
    for (auto iter = minions.begin(); iter != minions.end(); iter++) {
        auto m = *iter;

        for(auto iter2 = iter + 1; iter2 != minions.end(); iter2++){
            auto m2 = *iter2;
            auto ci = cd.checkCircleCircleCollision(*m, *m2);
            if(ci->isCollision){
                cr.doCollisionResponse(*m, *m2, ci);
            }
        }

        m->setAcceleration(glm::vec2(distrAcc(rng), distrAcc(rng)));
        m->setAngleAcc(distrAngleAcc(rng));
        
        m->update(dt);
    }
}

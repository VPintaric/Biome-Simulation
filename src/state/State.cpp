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
    std::uniform_real_distribution<float> angleDistr(0.f, 360.f);
    nMinions = n;

    Renderer &r = Renderer::getInstance();

    for(int i = 0; i < n; i++){
        std::shared_ptr<Minion> minion = std::make_shared<Minion>(r.getModel(SimConst::MINION_MODEL_NAME));
        minion->setRadius(10.f);
        minion->setPos(glm::vec2(posDistr(rng), posDistr(rng)));
        minion->setColor(glm::vec4(colDistr(rng), colDistr(rng), colDistr(rng), 1.f));
        minion->setAngle(angleDistr(rng));
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

    std::normal_distribution<float> distrAngleAcc(0.f, 200.f);
    std::normal_distribution<float> distrAcc(0.f, 10.f);
    for (auto iter = minions.begin(); iter != minions.end(); iter++) {
        auto m = *iter;

        for(auto iter2 = iter + 1; iter2 != minions.end(); iter2++){
            auto m2 = *iter2;
            auto ci = cd.checkCircleCircleCollision(*m, *m2);
            if(ci->isCollision){
                Log().Get(logINFO) << "Got collision!";
                cr.doCollisionResponse(*m, *m2, ci);
            }
        }

        m->setAcceleration(glm::vec2(distrAcc(rng), distrAcc(rng)));
        m->setAngleAcc(distrAngleAcc(rng));
        
        m->update(dt);
    }
}

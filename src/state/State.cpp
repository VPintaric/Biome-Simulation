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
#include "../rendering/Camera.h"

State& State::getInstance() {
    static State instance;
    return instance;
 }

State::State() {
    Log().Get(logDEBUG) << "Creating new state instance";
    shouldEndProgramFlag = false;
    rng.seed(static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
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
    std::uniform_real_distribution<float> posDistr((float) -WindowConst::WINDOW_HEIGHT / 2.f, (float) WindowConst::WINDOW_HEIGHT / 2.f);
    std::uniform_real_distribution<float> colDistr(0.2f, 1.f);
    std::uniform_real_distribution<float> angleDistr(0.f, glm::pi<float>());
    std::normal_distribution<float> radiusDistr(20.f, 5.f);
    std::normal_distribution<float> massDistr(1.f, 0.5f);

    Renderer &r = Renderer::getInstance();

//    std::shared_ptr<Minion> minion = std::make_shared<Minion>();
//    minion->setPos(glm::vec2(-200.f, 0.f));
//    minion->setSkinColor(glm::vec4(colDistr(rng), colDistr(rng), colDistr(rng), 1.f));
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
//    minion->setSkinColor(glm::vec4(colDistr(rng), colDistr(rng), colDistr(rng), 1.f));
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
        minion->setSkinColor(glm::vec4(colDistr(rng), colDistr(rng), colDistr(rng), 1.f));
        minion->setAngle(angleDistr(rng));
        minion->setRadius(radiusDistr(rng));
        minion->setRMass(10.f / (minion->getRadius() + massDistr(rng)));
        minion->setAngleVel(0.f);
        minion->setAngleAcc(0.f);
        minion->setDecay(5.f);
        minion->setMaxLife(100.f);
        minion->setLife(100.f);

        minions.push_back(minion);
    }
}

void State::initBoundary(float r) {
    boundary = std::make_shared<Boundary>();
    boundary->setR2(r);
    boundary->setPos(glm::vec2(0.f, 0.f));
    boundary->setColor(glm::vec3(1.f, 0.f, 0.f));
}

void State::draw() {
    for(const std::shared_ptr<Minion> &m : minions){
        m->draw();
    }
    boundary->draw();
}

void State::update(float dt) {
    CollisionDetection cd = CollisionDetection::getInstance();
    CollisionResponse cr = CollisionResponse::getInstance();

    for (auto iter = minions.begin(); iter != minions.end(); iter++) {
        auto m = *iter;

        for(auto iter2 = iter + 1; iter2 != minions.end(); iter2++){
            auto m2 = *iter2;
            auto ci = cd.checkCircleCircleCollision(*m, *m2);
            if(ci->isCollision){
                cr.doCollisionResponse(*m, *m2, ci);
            }
        }
        auto ci = cd.checkCircleHollowCollision(*m, *boundary);
        if(ci->isCollision){
            cr.doCollisionResponse(*m, *boundary, ci);
        }

        m->update(dt);
    }

    boundary->update(dt);

    Camera &c = Camera::getInstance();
    c.update(dt);
}

std::reference_wrapper< std::default_random_engine > State::getRng(){
    return std::ref(rng);
}

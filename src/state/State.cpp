#define GLM_FORCE_RADIANS

#include <random>
#include <chrono>
#include "glm/gtc/constants.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "state/State.h"
#include "state/Log.h"
#include "rendering/Renderer.h"
#include "../../include/collision/CollisionInfo.h"
#include "../../include/collision/CollisionDetection.h"
#include "../../include/collision/CollisionResponse.h"
#include "rendering/Camera.h"
#include "minion/factories/MinionGenerator.h"
#include "minion/controllers/BraveController.h"
#include "minion/controllers/NullController.h"

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

//    auto minion = std::make_shared<Minion>();
//
//    auto object = std::make_shared<MinionObject>();
//    object->setPos(glm::vec2(-40.f, 0.f));
//    object->setSkinColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
//    object->setAngle(3.f * glm::quarter_pi<float>());
//    object->setRadius(15.f);
//    object->setRMass(20.f / (object->getRadius()));
//    object->setAngleVel(0.f);
//    object->setAngleAcc(0.f);
//    object->setVelocity(glm::vec2(0.f, 0.f));
//    object->setAcceleration(glm::vec2(0.f, 0.f));
//    object->setDecay(2.5f);
//    object->setMaxLife(100.f);
//    object->setLife(100.f);
//
//    minion->setObject(object);
//    minion->setSenses(std::make_shared<MinionSenses>());
//    minion->setController(std::make_shared<BraveController>());
//
//    minions.push_back(minion);
//
//    minion = std::make_shared<Minion>();
//
//    object = std::make_shared<MinionObject>();
//    object->setPos(glm::vec2(40.f, 0.f));
//    object->setSkinColor(glm::vec4(0.1f, 0.1f, 0.5f, 1.f));
//    object->setAngle(glm::half_pi<float>());
//    object->setRadius(15.f);
//    object->setRMass(20.f / (object->getRadius()));
//    object->setAngleVel(0.f);
//    object->setAngleAcc(0.f);
//    object->setVelocity(glm::vec2(0.f, 0.f));
//    object->setAcceleration(glm::vec2(0.f, 0.f));
//    object->setDecay(2.5f);
//    object->setMaxLife(100.f);
//    object->setLife(100.f);
//
//    minion->setObject(object);
//    minion->setSenses(std::make_shared<MinionSenses>());
//    minion->setController(std::make_shared<NullController>());
//
//    minions.push_back(minion);

    MinionGenerator& mg = MinionGenerator::getInstance();
    for(int i = 0; i < n; i++){
        auto minion = mg.generateMinion();
        minions.push_back(minion);
    }
}

void State::initBoundary(float r) {
    boundary = std::make_shared<Boundary>();
    boundary->setR2(r);
    boundary->setPos(glm::vec2(0.f, 0.f));
    boundary->setColor(glm::vec4(1.f, 0.f, 0.f, 1.f));
}

void State::draw() {
    for(const std::shared_ptr<Minion> &m : minions){
        m->draw();
    }
    boundary->draw();
}

void State::update(float dt) {
    const float COLLISION_PUNISH = 1.f;
    const float BOUNDARY_COLLISION_PUNISH = 1.f;

    CollisionDetection cd = CollisionDetection::getInstance();
    CollisionResponse cr = CollisionResponse::getInstance();

    for (auto iter = minions.begin(); iter != minions.end(); iter++) {
        auto m = *iter;

        for(auto iter2 = iter + 1; iter2 != minions.end(); iter2++){
            auto m2 = *iter2;
            auto ci = cd.checkCircleCircleCollision(*m->getObject(), *m2->getObject(), true);

            float distance = 0.f;
            if(ci->isCollision){
                auto m1Obj = m->getObject();
                auto m2Obj = m2->getObject();

                cr.doCollisionResponse(*m->getObject(), *m2->getObject(), ci);

                if(!m1Obj->isDead() && !m2Obj->isDead()){
                    m1Obj->setLife(m1Obj->getLife() - COLLISION_PUNISH);
                    m2Obj->setLife(m2Obj->getLife() - COLLISION_PUNISH);
                } else if(m1Obj->isDead() && !m2Obj->isDead()){
                    m1Obj->setLife(m1Obj->getLife() - COLLISION_PUNISH);
                    m2Obj->setLife(m2Obj->getLife() + COLLISION_PUNISH);
                } else if(!m1Obj->isDead() && m2Obj->isDead()){
                    m1Obj->setLife(m1Obj->getLife() + COLLISION_PUNISH);
                    m2Obj->setLife(m2Obj->getLife() - COLLISION_PUNISH);
                }
            } else {
                distance = glm::length(ci->mtd);
            }

            auto senses = m->getSenses();
            if(!m->getObject()->isDead() && distance <= senses->MAX_SENSE_DISTANCE){
                auto s1 = std::make_shared<MinionSenses::SenseData>();
                s1->dist = distance;
                s1->color = m2->getObject()->getSkinColor();
                s1->angle = glm::orientedAngle(-ci->normal, m->getObject()->getFront());

                m->getSenses()->addSenseData(s1);
            }

            senses = m2->getSenses();
            if(!m2->getObject()->isDead() && distance <= senses->MAX_SENSE_DISTANCE){
                auto s2 = std::make_shared<MinionSenses::SenseData>();
                s2->dist = distance;
                s2->color = m->getObject()->getSkinColor();
                s2->angle = glm::orientedAngle(ci->normal, m2->getObject()->getFront());
                m2->getSenses()->addSenseData(s2);
            }
        }

        auto ci = cd.checkCircleHollowCollision(*m->getObject(), *boundary);
        if(ci->isCollision){
            cr.doCollisionResponse(*m->getObject(), *boundary, ci);

            m->getObject()->setLife(m->getObject()->getLife() - BOUNDARY_COLLISION_PUNISH);
        }

        m->update(dt);
        if(m->getObject()->isDecayed()){
            *iter = MinionGenerator::getInstance().generateMinion();
        }
    }

    boundary->update(dt);

    Camera &c = Camera::getInstance();
    c.update(dt);
}

std::reference_wrapper< std::default_random_engine > State::getRng(){
    return std::ref(rng);
}

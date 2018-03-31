#define GLM_FORCE_RADIANS

#include <random>
#include <chrono>
#include "glm/gtc/constants.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "state/State.h"
#include "state/Log.h"
#include "rendering/Renderer.h"
#include "../../include/collision/CollisionDetection.h"
#include "../../include/collision/CollisionResponse.h"
#include "rendering/Camera.h"

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

void State::setMinionGenerator(std::shared_ptr<MinionGenerator> gen) {
    minionGenerator = std::move(gen);
}

State::~State() {
    Log().Get(logDEBUG) << "Destroying state instance";
}

void State::initializeMinion(Minion &minion) {
    CollisionDetection cd = CollisionDetection::getInstance();
    const auto &object = minion.getObject();

    std::uniform_real_distribution<float> angleDistr(0.f, glm::two_pi<float>());
    std::uniform_real_distribution<float> distanceDistr(0.f, boundary->getR1() - object->getRadius());

    minion.setTimeLived(0.f);
    minion.setDecay(2.5f);
    minion.setMaxLife(100.f);
    minion.setMinLife(-100.f);
    minion.setLife(100.f);

    object->setVelocity(glm::vec2(0.f, 0.f));
    object->setAcceleration(glm::vec2(0.f, 0.f));
    object->setAngleVel(0.f);
    object->setAngleAcc(0.f);
    object->setControlForce(0.f);
    object->setControlRotMoment(0.f);

    int nTries = 0;
    const int TRIES_BEFORE_WARNING = 10;
    bool validPosition = false;

    while(!validPosition){
        ++nTries;
        if(nTries > TRIES_BEFORE_WARNING){
            // FIXME: if this proves to be a problem instead of randomly generating a new position move the minion a bit
            // out of the collision using the returned ci->mtd attribute
            Log().Get(logWARNING) << "Large amount of tries to initialize a non-colliding initial position for a minion!"
                                  "Current number of tries: " << nTries;
        }

        float angle = angleDistr(rng);
        float dist = distanceDistr(rng);

        object->setPos(glm::vec2(dist * glm::cos(angle), dist * glm::sin(angle)));
        object->setAngle(angleDistr(rng));

        validPosition = true;
        for (auto &minion : minions) {
            // hacky...
            if(minion->getObject() == object){
                continue;
            }
            auto ci = cd.checkCircleCircleCollision(*object, *minion->getObject());
            if(ci->isCollision){
                nTries++;
                validPosition = false;
                break;
            }
        }
    }
}

void State::endProgram(){
    this->shouldEndProgramFlag = true;
}

bool State::getShouldEndProgram() const {
    return this->shouldEndProgramFlag;
}

void State::spawnMinions(int n) {
    if(minionGenerator == nullptr){
        Log().Get(logWARNING) << "State has no reference to a minion generator object, unable to spawn minions";
        return;
    }

    for(int i = 0; i < n; i++){
        auto minion = minionGenerator->generateMinion();
        minions.push_back(minion);
        initializeMinion(*minion);
    }
}

void State::initBoundary(float r) {
    boundary = std::make_shared<Boundary>();
    boundary->setR2(r);
    boundary->setPos(glm::vec2(0.f, 0.f));
    boundary->setAngle(0.f);
    boundary->setVelocity(glm::vec2(0.f, 0.f));
    boundary->setAcceleration(glm::vec2(0.f, 0.f));
    boundary->setAngleVel(0.f);
    boundary->setAngleAcc(0.f);
    boundary->setColor(glm::vec4(1.f, 0.f, 0.f, 1.f));
}

void State::draw() {
    for(const std::shared_ptr<Minion> &m : minions){
        m->draw();
    }
    boundary->draw();
}

const std::vector< std::shared_ptr<Minion> > &State::getMinions() const{
    return minions;
}

std::shared_ptr<Boundary> State::getBoundary() const {
    return boundary;
}

void State::controlMinions(float dt) {
    for(auto minion : minions){
        minion->control(dt);
    }
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
            auto ci = cd.checkCircleCircleCollision(*m->getObject(), *m2->getObject());

            if(ci->isCollision){
                cr.doCollisionResponse(*m->getObject(), *m2->getObject(), ci);

                if(!m->isDead() && !m2->isDead()){
                    m->setLife(m->getLife() - COLLISION_PUNISH);
                    m2->setLife(m2->getLife() - COLLISION_PUNISH);
                } else if(m->isDead() && !m2->isDead()){
                    m->setLife(m->getLife() - COLLISION_PUNISH);
                    m2->setLife(m2->getLife() + COLLISION_PUNISH);
                } else if(!m->isDead() && m2->isDead()){
                    m->setLife(m->getLife() + COLLISION_PUNISH);
                    m2->setLife(m2->getLife() - COLLISION_PUNISH);
                }
            }
        }

        auto ci = cd.checkCircleHollowCollision(*m->getObject(), *boundary);
        if(ci->isCollision){
            cr.doCollisionResponse(*m->getObject(), *boundary, ci);

            m->setLife(m->getLife() - BOUNDARY_COLLISION_PUNISH);
        }

        m->update(dt);
        if(m->isDecayed()){
            *iter = minionGenerator->generateMinion();
            initializeMinion(**iter);
        }
    }

    boundary->update(dt);

    Camera &c = Camera::getInstance();
    c.update(dt);
}

std::reference_wrapper< std::default_random_engine > State::getRng(){
    return std::ref(rng);
}

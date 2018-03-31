#include <constants/SimulationConstants.h>
#include "minion/Minion.h"

Minion::Minion() : dead(false), decayed(false), id(0), timeLived(0.f){

};
Minion::~Minion() = default;

void Minion::update(float deltaT) {
    if(!isDead()){
        setLife(life - deltaT * decay);
    } else {
        setLife(life - deltaT * SimConst::MINION_BODY_DECAY_RATE);
    }

    if(life <= 0.f){
        setDead(true);
    }

    if(!isDead()){
        setTimeLived(getTimeLived() + deltaT);
    }

    if(life <= minLife){
        setDecayed(true);
    }

    object->update(deltaT);
}

void Minion::control(float deltaT) {
    senses->setAngle(object->getAngle());
    senses->setPos(object->getPos());
    senses->setAngleVel(object->getAngleVel());
    senses->setVelocity(object->getVelocity());
    senses->setRMass(object->getRMass());

    if(!isDead()){
        std::vector<float> controlParams = controller->controlMinion(senses->gatherData(deltaT));
        while(controlParams.size() < 2){
            controlParams.push_back(0.f);
        }

        object->setControlForce(controlParams[0]);
        object->setControlRotMoment(controlParams[1]);

        float lifeDecay = (SimConst::MINION_DECAY_RATE_FORCE_FACTOR * std::abs(controlParams[0]) +
                           SimConst::MINION_DECAY_RATE_MOMENT_FACTOR * std::abs(controlParams[1])) * decay;
        setLife(getLife() - lifeDecay * deltaT);
    } else {
        object->setControlForce(0.f);
        object->setControlRotMoment(0.f);
    }
}

void Minion::draw() {
    object->draw();
    if(!isDead()){
        senses->draw();
    }
}

const std::shared_ptr<MinionObject> &Minion::getObject() const {
    return object;
}

void Minion::setObject(const std::shared_ptr<MinionObject> &object) {
    Minion::object = object;
}

const std::shared_ptr<MinionSenses> &Minion::getSenses() const {
    return senses;
}

void Minion::setSenses(const std::shared_ptr<MinionSenses> &senses) {
    Minion::senses = senses;
}

const std::shared_ptr<MinionController> &Minion::getController() const {
    return controller;
}

void Minion::setController(const std::shared_ptr<MinionController> &controller) {
    Minion::controller = controller;
}

float Minion::getMaxLife() const {
    return maxLife;
}

void Minion::setMaxLife(float maxLife) {
    Minion::maxLife = maxLife;
}

float Minion::getMinLife() const {
    return minLife;
}

void Minion::setMinLife(float minLife) {
    Minion::minLife = minLife;
}

float Minion::getLife() const {
    return life;
}

void Minion::setLife(float life) {
    this->life = life;

    if(life > 0.f){
        setDecayed(false);
        setDead(false);
        if(life > maxLife){
            this->life = maxLife;
        }
    } else {
        setDead(true);
        setDecayed(!(life <= 0.f && life > minLife));
    }
}

float Minion::getDecay() const {
    return decay;
}

void Minion::setDecay(float decay) {
    Minion::decay = decay;
}

float Minion::getTimeLived() const {
    return timeLived;
}

void Minion::setTimeLived(float timeLived) {
    Minion::timeLived = timeLived;
}

bool Minion::isDead() const {
    return dead;
}

void Minion::setDead(bool dead) {
    Minion::dead = dead;
    if(isDead()){
        object->setBodyColor(glm::vec4(1.f, 1.f, 1.f, 1.f - (life / minLife)));
    } else {
        object->setBodyColor(glm::vec4(1.f - life / maxLife, life / maxLife, 0.f, 1.f));
    }
}

bool Minion::isDecayed() const {
    return decayed;
}

void Minion::setDecayed(bool decayed) {
    Minion::decayed = decayed;
}

int Minion::getId() const {
    return id;
}

void Minion::setId(int id){
    this->id = id;
}
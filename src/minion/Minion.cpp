#include <constants/SimulationConstants.h>
#include <state/Log.h>
#include <helpers/MathHelpers.h>
#include "minion/Minion.h"

Minion::Minion() : dead(false), decayed(false), id(0), timeLived(0.f), lastSavedPosition(-10000.f, -10000.f),
                    lastSavedPositionAge(0.f){

};

Minion::~Minion() = default;

void Minion::persistToJSON(rjs::Value &root, rjs::Document::AllocatorType& alloc) {
    root.SetObject();

    rjs::Value objectJSON(rjs::kObjectType);
    object->persistToJSON(objectJSON, alloc);
    root.AddMember(rjs::StringRef(JSON_OBJECT), objectJSON, alloc);

    rjs::Value sensesJSON(rjs::kObjectType);
    senses->persistToJSON(sensesJSON, alloc);
    root.AddMember(rjs::StringRef(JSON_SENSES), sensesJSON, alloc);

    rjs::Value controllerJSON(rjs::kObjectType);
    controller->persistToJSON(controllerJSON, alloc);
    root.AddMember(rjs::StringRef(JSON_CONTROLLER), controllerJSON, alloc);
}

void Minion::initFromJSON(rjs::Value &root) {
    auto objectJSON = root.FindMember(rjs::StringRef(JSON_OBJECT));
    if(objectJSON == root.MemberEnd() || !objectJSON->value.IsObject()){
        Log().Get(logWARNING) << "\"" << JSON_OBJECT << "\" not found in JSON, can't initialize";
    } else {
        object->initFromJSON(objectJSON->value);
    }

    auto sensesJSON = root.FindMember(rjs::StringRef(JSON_SENSES));
    if(sensesJSON == root.MemberEnd() || !sensesJSON->value.IsObject()){
        Log().Get(logWARNING) << "\"" << JSON_SENSES << "\" not found in JSON, can't initialize";
    } else {
        senses->initFromJSON(sensesJSON->value);
    }

    auto controllerJSON = root.FindMember(rjs::StringRef(JSON_CONTROLLER));
    if(controllerJSON == root.MemberEnd() || !controllerJSON->value.IsObject()){
        Log().Get(logWARNING) << "\"" << JSON_CONTROLLER << "\" not found in JSON, can't initialize";
    } else {
        controller->initFromJSON(controllerJSON->value);
    }
}

void Minion::update(float deltaT) {
    const float MAX_LAST_SAVED_POSITION_AGE = 1.f;
    const float MAX_DISTANCE_TRAVELED_POINTS_ADDED = 150.f;

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

    if(!isDead()){
        lastSavedPositionAge += deltaT;
        if(lastSavedPositionAge >= MAX_LAST_SAVED_POSITION_AGE){
            float distance = glm::length(lastSavedPosition - object->getPos());
            distance = Math::clamp(distance, 0.f, MAX_DISTANCE_TRAVELED_POINTS_ADDED);
            Log().Get(logDEBUG) << distance;
            setDistanceTraveledPoints(getDistanceTraveledPoints() + distance);
            lastSavedPosition = object->getPos();
            lastSavedPositionAge = 0.f;
        }
    }
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

        float lifeDecay = (SimConst::MINION_DECAY_RATE_FORCE_FACTOR * std::abs(object->getControlForce()) +
                           SimConst::MINION_DECAY_RATE_MOMENT_FACTOR * std::abs(object->getControlRotMoment())) * decay;
        setLife(getLife() - lifeDecay * deltaT);
    } else {
        object->setControlForce(0.f);
        object->setControlRotMoment(0.f);
    }
}

void Minion::draw(bool drawSenses) {
    object->draw();
    if(drawSenses && !isDead()){
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

float Minion::getDistanceTraveledPoints() const {
    return distanceTraveledPoints;
}

void Minion::setDistanceTraveledPoints(float distanceTraveledPoints) {
    Minion::distanceTraveledPoints = distanceTraveledPoints;
}

float Minion::getDamageDealt() const {
    return damageDealt;
}

void Minion::setDamageDealt(float damageDealt) {
    Minion::damageDealt = damageDealt;
}

float Minion::getHealthRecovered() const {
    return healthRecovered;
}

void Minion::setHealthRecovered(float healthRecovered) {
    Minion::healthRecovered = healthRecovered;
}

float Minion::getFitness() const {
    return fitness;
}

void Minion::setFitness(float fitness) {
    Minion::fitness = fitness;
}

std::shared_ptr<Minion> Minion::copy() {
    auto newCopy = std::make_shared<Minion>();

    newCopy->senses = senses->copy();
    newCopy->senses->setMinion(newCopy);

    newCopy->controller = controller->copy();
    newCopy->controller->setMinion(newCopy);

    newCopy->object = object->copy();

    return newCopy;
}

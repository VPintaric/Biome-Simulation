#define GLM_FORCE_RADIANS

#include <random>
#include <chrono>
#include <constants/SimulationConstants.h>
#include <helpers/MathHelpers.h>
#include "glm/gtc/constants.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "state/State.h"
#include "state/Log.h"
#include "rendering/Renderer.h"
#include "../../include/collision/CollisionDetection.h"
#include "../../include/collision/CollisionResponse.h"
#include "rendering/Camera.h"
#include <persistence/Persistence.h>
#include <experimental/filesystem>
#include <minion/selection/CurrentLongestLivingSelection.h>
#include <minion/factories/explicit/ExplicitBehaviourMinionGenerator.h>
#include <minion/factories/neuralnet/NeuralNetMinionGenerator.h>

namespace fs = std::experimental::filesystem;
namespace chr = std::chrono;

State& State::getInstance() {
    static State instance;
    return instance;
 }

State::State() : nextMinionId(1), currentBestMinion(nullptr),
                nextPersistedGeneration(1), persistenceDirectory("saved_minions"),
                nMinions(SimConst::DEFAULT_NUMBER_OF_MINIONS), printEveryRealTime(30000),
                persistMinionsEveryRealTime(300000), nextPersistTimestamp(persistMinionsEveryRealTime),
                nextPrintTimestamp(printEveryRealTime), useGenerationalGA(true){
    Log().Get(logDEBUG) << "Creating new state instance";
    shouldEndProgramFlag = false;
    lastCalledTimestamp = chr::duration_cast<chr::milliseconds>(chr::system_clock::now().time_since_epoch()).count();
    rng.seed(static_cast<unsigned long>(lastCalledTimestamp));
    Log().Get(logDEBUG) << "Created new state instance";
}

void State::setMinionGenerator(std::shared_ptr<MinionGenerator> gen) {
    minionGenerator = std::move(gen);
    if(selectionAlg){
        selectionAlg->setMinionGenerator(minionGenerator);
    }
}

void State::setSelectionAlg(std::shared_ptr<Selection> sel) {
    selectionAlg = std::move(sel);
    selectionAlg->setMinionGenerator(minionGenerator);
}

State::~State() {
    Log().Get(logDEBUG) << "Destroying state instance";
}

void State::initializeMinion(Minion &minion) {
    CollisionDetection cd = CollisionDetection::getInstance();
    const auto &object = minion.getObject();
    const auto &senses = minion.getSenses();

    std::uniform_real_distribution<float> angleDistr(0.f, glm::two_pi<float>());
    std::uniform_real_distribution<float> distanceDistr(0.f, boundary->getR1() - object->getRadius());

    auto c = object->getSkinColor();
    object->setSkinColor(glm::vec4(Math::clamp(c.r, 0.f, 1.f), Math::clamp(c.g, 0.f, 1.f), Math::clamp(c.b, 0.f, 1.f), 1.f));

    senses->setMaxSenseDistance(Math::clamp(senses->getMaxSenseDistance(), SimConst::MINION_MIN_MAX_SENSE_DISTANCE,
                                            SimConst::MINION_MAX_MAX_SENSE_DISTANCE));
    minion.setDecay(SimConst::MINION_DECAY_RATE_SENSE_DISTANCE_FACTOR * senses->getMaxSenseDistance() +
                            SimConst::MINION_DEFAULT_DECAY_RATE);
    object->setRadius(Math::clamp(object->getRadius(), SimConst::MINION_MIN_RADIUS, SimConst::MINION_MAX_RADIUS));
    object->setRMass(SimConst::MINION_RADIUS_TO_RMASS_RATIO / object->getRadius());
    minion.setMaxLife(SimConst::MINION_MAX_LIFE_RADIUS_FACTOR * object->getRadius());
    minion.setMinLife(-minion.getMaxLife());

    minion.setTimeLived(0.f);
    minion.setLife(minion.getMaxLife());
    minion.setId(nextMinionId++);

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
            if(minion->isDead() || minion->getObject() == object){
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

void State::spawnMinions() {
    if(minionGenerator == nullptr){
        Log().Get(logWARNING) << "State has no reference to a minion generator object, unable to spawn minions";
        return;
    }

    if(!loadDirectory.empty()){
        loadMinionsFromFolder(loadDirectory);
    } else {
        for(int i = 0; i < nMinions; i++){
            auto minion = minionGenerator->generateMinion();
            minions.push_back(minion);
            initializeMinion(*minion);
        }
    }

    decayedMinions.clear();
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
        if(!m->isDecayed()){
            m->draw();
        }
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
    for(const auto &minion : minions){
        minion->control(dt);
    }
}

void State::resolveMinionCollisionDamage(Minion &m1, Minion &m2, CollisionInfo &ci) {
    if(m1.isDead() && m2.isDead()){
        return;
    } else if(!m1.isDead() && !m2.isDead()){
        const float COLLISION_DMG_CONSTANT = 3e-4f;

        const auto &obj1 = m1.getObject();
        const auto &obj2 = m2.getObject();

        glm::vec2 relativeVelocity = obj1->getVelocity() - obj2->getVelocity();

        glm::vec2 collisionDirection = ci.p1 - obj1->getPos();
        float dmgTo2 = COLLISION_DMG_CONSTANT *
                        std::max(0.f, glm::dot(collisionDirection, obj1->getFront())) *
                        std::max(0.f, glm::dot(collisionDirection, relativeVelocity)) *
                        obj2->getRMass();

        collisionDirection = ci.p2 - obj2->getPos();
        float dmgTo1 = COLLISION_DMG_CONSTANT *
                        std::max(0.f, glm::dot(collisionDirection, obj2->getFront())) *
                        std::max(0.f, glm::dot(collisionDirection, -relativeVelocity)) *
                        obj1->getRMass();

        m1.setLife(m1.getLife() - dmgTo1);
        m2.setLife(m2.getLife() - dmgTo2);
    } else {
        const float COLLISION_LEECH_CONSTANT = 0.5f;

        if(m1.isDead()){
            m1.setLife(m1.getLife() - COLLISION_LEECH_CONSTANT);
            m2.setLife(m2.getLife() + COLLISION_LEECH_CONSTANT);
        } else {
            m1.setLife(m1.getLife() + COLLISION_LEECH_CONSTANT);
            m2.setLife(m2.getLife() - COLLISION_LEECH_CONSTANT);
        }
    }
}

void State::resolveMinionBoundaryCollisionDamage(Minion &m, CollisionInfo &ci) {
    const float COLLISION_DMG_BOUNDARY_COEFF = 1e-3f;
    const float BOUNDARY_CONSTANT_DAMAGE = 0.25f;

    if(!m.isDead()){
        glm::vec2 relativeVelocity = m.getObject()->getVelocity() - boundary->getVelocity();
        glm::vec2 collisionDirection = ci.p1 - m.getObject()->getPos();

        float dmg = BOUNDARY_CONSTANT_DAMAGE +
                    COLLISION_DMG_BOUNDARY_COEFF * std::max(0.f, glm::dot(collisionDirection, relativeVelocity));
        m.setLife(m.getLife() - dmg);
    }
}

void State::realTimeUpdate() {
    long timestamp = chr::duration_cast<chr::milliseconds>(chr::system_clock::now().time_since_epoch()).count();
    long dt = timestamp - lastCalledTimestamp;
    lastCalledTimestamp = timestamp;
    realTimePassed += dt;

    if(realTimePassed >= nextPrintTimestamp){
        Log().Get(logINFO) << "Time passed: " << (realTimePassed / 1000.0) << " seconds";
        nextPrintTimestamp += printEveryRealTime;
    }

    if(realTimePassed >= nextPersistTimestamp){
        persistCurrentGeneration();
        nextPersistTimestamp += persistMinionsEveryRealTime;
    }

    timestamp = chr::duration_cast<chr::milliseconds>(chr::system_clock::now().time_since_epoch()).count();
    realTimePassed += timestamp - lastCalledTimestamp;
    lastCalledTimestamp = timestamp;
}

void State::initializeNextGeneration() {
    currentGeneration++;
    Log().Get(logINFO) << "new gen";
    for(auto& m : minions){
        m = selectionAlg->getNewMinion();
        initializeMinion(*m);
    }
    decayedMinions.clear();
}

void State::update(float dt) {
    realTimeUpdate();

    CollisionDetection cd = CollisionDetection::getInstance();
    CollisionResponse cr = CollisionResponse::getInstance();

    for (auto iter = minions.begin(); iter != minions.end(); iter++) {
        auto m = *iter;

        for(auto iter2 = iter + 1; iter2 != minions.end(); iter2++){
            auto m2 = *iter2;
            auto ci = cd.checkCircleCircleCollision(*m->getObject(), *m2->getObject());

            if(ci->isCollision){
                resolveMinionCollisionDamage(*m, *m2, *ci);
                if(!m->isDead() && !m2->isDead() || m->isDead() && m2->isDead()) {
                    cr.doCollisionResponse(*m->getObject(), *m2->getObject(), ci);
                }
            }
        }

        auto ci = cd.checkCircleHollowCollision(*m->getObject(), *boundary);
        if(ci->isCollision){
            resolveMinionBoundaryCollisionDamage(*m, *ci);
            cr.doCollisionResponse(*m->getObject(), *boundary, ci);
        }

        m->update(dt);
        if(m->isDecayed()){
            if(currentBestMinion == nullptr || currentBestMinion->getTimeLived() < m->getTimeLived()){
                currentBestMinion = m;
                Log().Get(logINFO) << "New longest living time: " << currentBestMinion->getTimeLived();
            }

            if(!useGenerationalGA){
                *iter = selectionAlg->getNewMinion();
                initializeMinion(**iter);
                currentGeneration++;
            } else {
                decayedMinions.insert(*iter);

                if(decayedMinions.size() == minions.size()){
                    initializeNextGeneration();
                }
            }
        }
    }

    boundary->update(dt);

    Camera &c = Camera::getInstance();
    c.update(dt);
}

std::reference_wrapper< std::default_random_engine > State::getRng(){
    return std::ref(rng);
}

void State::setPersistenceDirectory(std::string dirName) {
    persistenceDirectory = std::move(dirName);
}

std::string State::getPersistenceDirectory() {
    return persistenceDirectory;
}

void State::persistCurrentGeneration() {
    if(persistenceDirectory.empty()){
        Log().Get(logINFO) << "No save directory given, not persisting minions";
        return;
    }

    auto& p = Persistence::getInstance();

    if(!fs::exists(persistenceDirectory) || !fs::is_directory(persistenceDirectory)){
        fs::create_directory(persistenceDirectory);
    }

    std::string genDir = persistenceDirectory + "/generation_" + std::to_string(nextPersistedGeneration);

    while(fs::exists(genDir) && fs::is_directory(genDir)){
        nextPersistedGeneration++;
        genDir = persistenceDirectory + "/generation_" + std::to_string(nextPersistedGeneration);
    }
    fs::create_directory(genDir);

    Log().Get(logINFO) << "Persisting current generation to folder: " << genDir;
    for(const auto &minion : minions){
        p.saveMinionToFile(genDir + "/minion_" + std::to_string(minion->getId()), minion);
    }

    nextPersistedGeneration++;
}

void State::loadMinionsFromFolder(std::string dirName) {
    auto& p = Persistence::getInstance();

    if(!fs::exists(dirName) || !fs::is_directory(dirName)){
        Log().Get(logWARNING) << "Directory " << dirName << " does not exist";
        return;
    }

    minions.clear();
    for(auto it = fs::directory_iterator(dirName); it != fs::directory_iterator(); ++it){
        std::string fileName = (*it).path().string();
        auto minion = minionGenerator->generateMinion();
        p.initMinionFromFile(fileName, minion);
        initializeMinion(*minion);
        minions.push_back(minion);
    }
}

void State::configureFromJSON(rjs::Value &root) {
    const char * MINION_GEN = "minion_generator";
    const char * BOUNDARY_RADIUS = "boundary_radius";
    const char * N_MINIONS = "number_of_minions";
    const char * MINION_GEN_CONFIG = "minion_generator_config";
    const char * PRINT_INFO_EVERY_SECONDS = "print_info_every_seconds";
    const char * PERSIST_MINIONS_EVERY_SECONDS = "persist_minions_every_seconds";
    const char * PERSISTENCE_DIRECTORY = "persistence_directory";
    const char * EVOLUTION_TYPE = "evolution_type";
    const char * EVOLUTION_STEADY_STATE_TYPE = "steady_state";
    const char * LOAD_MINIONS_DIRECTORY = "load_directory";
    const char * SAVE_MINIONS_DIRECTORY = "save_directory";

    if(!root.IsObject()){
        Log().Get(logWARNING) << "Can't get configuration from non-object value in JSON";
        return;
    }

    setSelectionAlg(std::make_shared<CurrentLongestLivingSelection>());

    if(root.HasMember(MINION_GEN) && root[MINION_GEN].IsString()){
        auto minionGenerator = root[MINION_GEN].GetString();
        if(minionGenerator == "explicit_behaviour"){
            setMinionGenerator(std::make_shared<ExplicitBehaviourMinionGenerator>());
        } else {
            setMinionGenerator(std::make_shared<NeuralNetMinionGenerator>());
        }

        if(root.HasMember(MINION_GEN_CONFIG) && root[MINION_GEN_CONFIG].IsObject()){
            this->minionGenerator->configureFromJSON(root[MINION_GEN_CONFIG]);
        }

    } else {
        setMinionGenerator(std::make_shared<NeuralNetMinionGenerator>());
    }

    if(root.HasMember(BOUNDARY_RADIUS) && root[BOUNDARY_RADIUS].IsNumber()){
        initBoundary(root[BOUNDARY_RADIUS].GetFloat());
    } else {
        initBoundary(SimConst::DEFAULT_BOUNDARY_RADIUS);
    }

    if(root.HasMember(N_MINIONS) && root[N_MINIONS].IsInt()){
        setNMinions(root[N_MINIONS].GetInt());
    } else {
        setNMinions(SimConst::DEFAULT_NUMBER_OF_MINIONS);
    }

    if(root.HasMember(PRINT_INFO_EVERY_SECONDS) && root[PRINT_INFO_EVERY_SECONDS].IsInt()){
        printEveryRealTime = root[PRINT_INFO_EVERY_SECONDS].GetInt() * 1000;
        nextPrintTimestamp = printEveryRealTime;
        realTimePassed = 0;
    }

    if(root.HasMember(PERSIST_MINIONS_EVERY_SECONDS) && root[PERSIST_MINIONS_EVERY_SECONDS].IsInt()){
        persistMinionsEveryRealTime = root[PERSIST_MINIONS_EVERY_SECONDS].GetInt() * 1000;
        nextPersistTimestamp = persistMinionsEveryRealTime;
        realTimePassed = 0;
    }

    if(root.HasMember(PERSISTENCE_DIRECTORY) && root[PERSISTENCE_DIRECTORY].IsString()){
        setPersistenceDirectory(root[PERSISTENCE_DIRECTORY].GetString());
    }

    if(root.HasMember(EVOLUTION_TYPE) && root[EVOLUTION_TYPE].IsString()){
        useGenerationalGA = !(root[EVOLUTION_TYPE] == EVOLUTION_STEADY_STATE_TYPE);
    }

    if(root.HasMember(LOAD_MINIONS_DIRECTORY) && root[LOAD_MINIONS_DIRECTORY].IsString()){
        loadDirectory = root[LOAD_MINIONS_DIRECTORY].GetString();
    } else {
        loadDirectory.clear();
    }

    if(root.HasMember(SAVE_MINIONS_DIRECTORY) && root[SAVE_MINIONS_DIRECTORY].IsString()){
        persistenceDirectory = root[SAVE_MINIONS_DIRECTORY].GetString();
    } else {
        persistenceDirectory.clear();
    }
}

int State::getNMinions() const {
    return nMinions;
}

void State::setNMinions(int nMinions) {
    State::nMinions = nMinions;
}

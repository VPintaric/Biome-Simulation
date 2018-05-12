#define GLM_FORCE_RADIANS

#include <random>
#include <chrono>
#include <constants/SimulationConstants.h>
#include <algorithm>
#include <helpers/MathHelpers.h>
#include "glm/gtc/constants.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "state/State.h"
#include "state/Log.h"
#include "rendering/Renderer.h"
#include "collision/CollisionDetection.h"
#include "collision/CollisionResponse.h"
#include "rendering/Camera.h"
#include <persistence/Persistence.h>
#include <experimental/filesystem>
#include <minion/factories/neuralnet/NeuralNetMinionGenerator.h>
#include <minion/fitness/TimeLivedFitness.h>
#include <minion/fitness/ActivityFitness.h>
#include <minion/selection/RoulleteWheelSelection.h>
#include <helpers/RNG.h>
#include <minion/crossover_operators/PickBetterMinionCrossover.h>
#include <minion/crossover_operators/PickRandomMinionCrossover.h>
#include <minion/crossover_operators/CustomMinionCrossover.h>
#include <minion/mutation_operators/DoNothingMinionMutation.h>
#include <minion/mutation_operators/CustomMinionMutation.h>
#include <minion/factories/decisiontree/DecisionTreeMinionGenerator.h>
#include <minion/factories/hardcoded/HardcodedMinionGenerator.h>
#include <minion/controllers/hardcoded/HardcodedController.h>

namespace fs = std::experimental::filesystem;
namespace chr = std::chrono;

State& State::getInstance() {
    static State instance;
    return instance;
 }

State::State() : nextMinionId(1), currentBestMinion(nullptr),
                nextPersistedGeneration(1), persistenceDirectory("saved_minions"),
                nMinions(SimConst::DEFAULT_NUMBER_OF_MINIONS), nElites(1), printEveryRealTime(30000),
                persistMinionsEveryRealTime(300000), nextPersistTimestamp(persistMinionsEveryRealTime),
                nextPrintTimestamp(printEveryRealTime), useGenerationalGA(true),
                nFoodPellets(0), nPoisonPellets(0), drawSenses(true), nDefaultMinions(0),
                hcGen(std::make_shared<HardcodedMinionGenerator>()){
    Log().Get(logDEBUG) << "Creating new state instance";
    shouldEndProgramFlag = false;
    lastCalledTimestamp = chr::duration_cast<chr::milliseconds>(chr::system_clock::now().time_since_epoch()).count();
    Log().Get(logDEBUG) << "Created new state instance";
}

void State::setMinionGenerator(std::shared_ptr<MinionGenerator> gen) {
    minionGenerator = std::move(gen);
}

void State::setSelectionAlg(std::shared_ptr<Selection> sel) {
    selectionAlg = std::move(sel);
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
    minion.setDamageDealt(0.f);
    minion.setDistanceTraveledPoints(0.f);
    minion.setHealthRecovered(0.f);
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

        float angle = angleDistr(RNG::get());
        float dist = distanceDistr(RNG::get());

        object->setPos(glm::vec2(dist * glm::cos(angle), dist * glm::sin(angle)));
        object->setAngle(angleDistr(RNG::get()));

        validPosition = true;
        for (auto &minion : allMinions) {
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
        Log().Get(logWARNING) << "State has no reference to a minion generator object, unable to spawn customMinions";
        return;
    }

    if(!loadDirectory.empty()){
        loadMinionsFromFolder(loadDirectory);
    } else {
        for(int i = 0; i < nMinions; i++){
            auto minion = minionGenerator->generateRandomMinion();
            customMinions.push_back(minion);
            allMinions.push_back(minion);
            initializeMinion(*minion);
        }
    }

    for(int i = 0; i < nDefaultMinions; i++){
        auto minion = hcGen->generateMinion();
        hcMinions.push_back(minion);
        allMinions.push_back(minion);
        initializeMinion(*minion);
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
    for(const auto& p : foodPellets){
        p->draw();
    }

    for(const auto& p : poisonPellets){
        p->draw();
    }

    for(const auto& m : allMinions){
        if(!m->isDecayed()){
            m->draw(drawSenses);
        }
    }
    boundary->draw();
}

const std::vector< std::shared_ptr<Minion> > &State::getMinions() const{
    return customMinions;
}

std::shared_ptr<Boundary> State::getBoundary() const {
    return boundary;
}

void State::controlMinions(float dt) {
    for(const auto &minion : allMinions){
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

        m1.setDamageDealt(m1.getDamageDealt() + dmgTo2);
        m2.setDamageDealt(m2.getDamageDealt() + dmgTo1);
    } else {
        const float COLLISION_LEECH_CONSTANT = 0.5f;

        if(m1.isDead() && !m1.isDecayed()){
            m1.setLife(m1.getLife() - COLLISION_LEECH_CONSTANT);
            m2.setLife(m2.getLife() + COLLISION_LEECH_CONSTANT);

            m2.setHealthRecovered(m2.getHealthRecovered() + COLLISION_LEECH_CONSTANT);
        } else if(m2.isDead() && !m2.isDecayed()){
            m1.setLife(m1.getLife() + COLLISION_LEECH_CONSTANT);
            m2.setLife(m2.getLife() - COLLISION_LEECH_CONSTANT);

            m1.setHealthRecovered(m1.getHealthRecovered() + COLLISION_LEECH_CONSTANT);
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

void State::updateMinionFitness() {
    currentFitnessAverage = 0.f;
    for(auto m : customMinions){
        fitnessAlg->calculateFitness(m);
        currentFitnessAverage += m->getFitness();
    }
    currentFitnessAverage /= customMinions.size();
}

void State::initializeNextGeneration() {
    Log().Get(logINFO) << "Final average fitness of " << currentGeneration << ". generation is " << currentFitnessAverage;
    currentGeneration++;
    Log().Get(logINFO) << "Generating " << currentGeneration << ". generation";

    allMinions.clear();
    allMinions.reserve(nMinions + nDefaultMinions);

    if(!customMinions.empty()){
        std::vector<std::shared_ptr<Minion> > newGeneration, bestInCurrentGen;
        newGeneration.reserve(nMinions);
        bestInCurrentGen.reserve(nElites);

        std::sort(customMinions.begin(), customMinions.end(),
                  [](std::shared_ptr<Minion> m1, std::shared_ptr<Minion> m2){
                      return m1->getFitness() > m2->getFitness();
                  });

        bestInCurrentGen.insert(bestInCurrentGen.begin(), customMinions.begin(), customMinions.begin() + nElites);
        newGeneration.insert(newGeneration.begin(), bestInCurrentGen.begin(), bestInCurrentGen.end());

        for(int i = 0; i < nMinions - nElites; i++){
            auto parents = selectionAlg->selectParents(customMinions);
            auto m = crossover->crossover(parents.first, parents.second);
            mutation->mutate(m);
            newGeneration.push_back(m);
        }
        customMinions = newGeneration;

        allMinions.insert(allMinions.end(), customMinions.begin(), customMinions.end());
    }

    if(!hcMinions.empty()){
        allMinions.insert(allMinions.end(), hcMinions.begin(), hcMinions.end());
    }

    for(auto m : allMinions){
        initializeMinion(*m);
    }

    decayedMinions.clear();
}

void State::update(float dt) {
    realTimeUpdate();

    CollisionDetection cd = CollisionDetection::getInstance();
    CollisionResponse cr = CollisionResponse::getInstance();

    updateMinionFitness();
    for (auto iter = allMinions.begin(); iter != allMinions.end(); iter++) {
        auto m = *iter;

        for(auto iter2 = iter + 1; iter2 != allMinions.end(); iter2++){
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

        for(const auto &pellet : foodPellets){
            auto ci = cd.checkCircleCircleCollision(*m->getObject(), *pellet);

            if(ci->isCollision){
                resolveMinionFoodPelletDamage(*m, *pellet, *ci);
                if(pellet->getLife() <= 0.f){
                    initializePellet(pellet);
                }
            }
        }

        for(const auto &pellet : poisonPellets){
            auto ci = cd.checkCircleCircleCollision(*m->getObject(), *pellet);

            if(ci->isCollision){
                resolveMinionPoisonPelletDamage(*m, *pellet, *ci);
                if(pellet->getLife() <= 0.f){
                    initializePellet(pellet);
                }
            }
        }

        m->update(dt);
        if(m->isDecayed()){
            // TODO: Hacky and expensive way of checking if it's a hardcoded controller minion...
            if(std::dynamic_pointer_cast<HardcodedController>(m->getController()) == nullptr &&
                    (currentBestMinion == nullptr || currentBestMinion->getFitness() < m->getFitness())){
                currentBestMinion = m->copy();
                currentBestMinion->setFitness(m->getFitness());
                Log().Get(logINFO) << "New best fitness minion: " << m->getFitness();
                Log().Get(logINFO) << "   Time lived: " << m->getTimeLived();
                Log().Get(logINFO) << "   Distance traveled: " << m->getDistanceTraveledPoints();
                Log().Get(logINFO) << "   Damage dealt: " << m->getDamageDealt();
                Log().Get(logINFO) << "   Health recovered: " << m->getHealthRecovered();
            }

            if(!useGenerationalGA){
                throw std::string("Tournament evolution is probably broken and probably does not work corectly anyway...");
//                auto parents = selectionAlg->selectParents(customMinions);
//                *iter = crossover->crossover(parents.first, parents.second);
//                mutation->mutate(*iter);
//                initializeMinion(**iter);
//                currentGeneration++;
            } else {
                decayedMinions.insert(*iter);

                if(decayedMinions.size() == allMinions.size()){
                    initializeNextGeneration();
                }
            }
        }
    }

    boundary->update(dt);

    Camera &c = Camera::getInstance();
    c.update(dt);
}

void State::setPersistenceDirectory(std::string dirName) {
    persistenceDirectory = std::move(dirName);
}

std::string State::getPersistenceDirectory() {
    return persistenceDirectory;
}

void State::persistCurrentGeneration() {
    if(persistenceDirectory.empty()){
        Log().Get(logINFO) << "No save directory given, not persisting customMinions";
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
    for(const auto &minion : customMinions){
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

    customMinions.clear();
    for(auto it = fs::directory_iterator(dirName); it != fs::directory_iterator(); ++it){
        std::string fileName = (*it).path().string();
        auto minion = minionGenerator->generateMinion();
        p.initMinionFromFile(fileName, minion);
        initializeMinion(*minion);
        customMinions.push_back(minion);
    }
}

void State::configureFromJSON(rjs::Value &root) {
    const char * FITNESS_ALGORITHM = "fitness";
    const char * FITNESS_ALGORITHM_CONFIG = "fitness_config";
    const char * MINION_GEN = "minion_generator";
    const char * BOUNDARY_RADIUS = "boundary_radius";
    const char * N_MINIONS = "number_of_minions";
    const char * MINION_GEN_CONFIG = "minion_generator_config";
    const char * PRINT_INFO_EVERY_SECONDS = "print_info_every_seconds";
    const char * PERSIST_MINIONS_EVERY_SECONDS = "persist_minions_every_seconds";
    const char * PERSISTENCE_DIRECTORY = "persistence_directory";
    const char * EVOLUTION_TYPE = "evolution_type";
    const char * ELITISM = "elitism";
    const char * EVOLUTION_STEADY_STATE_TYPE = "steady_state";
    const char * LOAD_MINIONS_DIRECTORY = "load_directory";
    const char * SAVE_MINIONS_DIRECTORY = "save_directory";
    const char * MUTATION_OPERATOR = "mutation_operator";
    const char * MUTATION_OPERATOR_CONFIG = "mutation_operator_config";
    const char * CROSSOVER_OPERATOR = "crossover_operator";
    const char * CROSSOVER_OPERATOR_CONFIG = "crossover_operator_config";
    const char * FOOD_PELLETS = "food_pellets";
    const char * POISON_PELLETS = "poison_pellets";
    const char * NUM_DEFAULT_MINIONS = "number_of_default_minions";

    Log().Get(logDEBUG) << "Configuring State object...";

    if(!root.IsObject()){
        Log().Get(logWARNING) << "Can't get configuration from non-object value in JSON";
        return;
    }

    Log().Get(logDEBUG) << "Using roullete wheel selection";
    setSelectionAlg(std::make_shared<RoulleteWheelSelection>());

    if(root.HasMember(FITNESS_ALGORITHM) && root[FITNESS_ALGORITHM].IsString()){
        std::string fitnessAlg = root[FITNESS_ALGORITHM].GetString();
        if(fitnessAlg == "time_alive"){
            Log().Get(logDEBUG) << "Using TimeLived function for fitness";
            setFitnessAlg(std::make_shared<TimeLivedFitness>());
        } else {
            Log().Get(logDEBUG) << "Using Activity function for fitness";
            setFitnessAlg(std::make_shared<ActivityFitness>());
        }
    } else {
        Log().Get(logDEBUG) << "Using Activity function for fitness";
        setFitnessAlg(std::make_shared<ActivityFitness>());
    }

    if(root.HasMember(FITNESS_ALGORITHM_CONFIG) && root[FITNESS_ALGORITHM_CONFIG].IsObject()){
        this->fitnessAlg->configureFromJSON(root[FITNESS_ALGORITHM_CONFIG]);
    }

    if(root.HasMember(MINION_GEN) && root[MINION_GEN].IsString()){
        std::string minionGen = root[MINION_GEN].GetString();
        if(minionGen == "decision_tree"){
            Log().Get(logDEBUG) << "Using decisionTreeMinionGenerator for generating customMinions";
            setMinionGenerator(std::make_shared<DecisionTreeMinionGenerator>());
        } else {
            Log().Get(logDEBUG) << "Using neuralNetMinionGenerator for generating customMinions";
            setMinionGenerator(std::make_shared<NeuralNetMinionGenerator>());
        }
    }

    if(root.HasMember(MINION_GEN_CONFIG) && root[MINION_GEN_CONFIG].IsObject()){
        minionGenerator->configureFromJSON(root[MINION_GEN_CONFIG]);
    }

    if(root.HasMember(BOUNDARY_RADIUS) && root[BOUNDARY_RADIUS].IsNumber()){
        initBoundary(root[BOUNDARY_RADIUS].GetFloat());
    } else {
        initBoundary(SimConst::DEFAULT_BOUNDARY_RADIUS);
    }
    Log().Get(logDEBUG) << "Boundary radius initialized to " << boundary->getR1();

    if(root.HasMember(N_MINIONS) && root[N_MINIONS].IsInt()){
        setNMinions(root[N_MINIONS].GetInt());
    } else {
        setNMinions(SimConst::DEFAULT_NUMBER_OF_MINIONS);
    }
    Log().Get(logDEBUG) << "Number of customMinions initalized to " << getNMinions();

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

    if(useGenerationalGA && root.HasMember(ELITISM) && root[ELITISM].IsInt()){
        nElites = root[ELITISM].GetInt();
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

    if(root.HasMember(CROSSOVER_OPERATOR) && root[CROSSOVER_OPERATOR].IsString()){
        std::string name = root[CROSSOVER_OPERATOR].GetString();
        std::stringstream ss;

        ss << "For crossover operator using: ";
        if(name == "pick_better"){
            ss << "PickBetter";
            crossover = std::make_shared<PickBetterMinionCrossover>();
        } else if(name == "pick_random"){
            ss << "PickRandom";
            crossover = std::make_shared<PickRandomMinionCrossover>();
        } else if(name == "custom"){
            ss << "Custom";
            crossover = std::make_shared<CustomMinionCrossover>();
        } else {
            ss << "PickBetter";
            crossover = std::make_shared<PickBetterMinionCrossover>();
        }
        Log().Get(logDEBUG) << ss.str();
    } else {
        Log().Get(logDEBUG) << "For crossover operator using: PickBetter";
        crossover = std::make_shared<PickBetterMinionCrossover>();
    }

    if(root.HasMember(CROSSOVER_OPERATOR_CONFIG) && root[CROSSOVER_OPERATOR_CONFIG].IsObject()){
        crossover->configureFromJSON(root[CROSSOVER_OPERATOR_CONFIG]);
    }

    if(root.HasMember(MUTATION_OPERATOR) && root[MUTATION_OPERATOR].IsString()){
        std::string name = root[MUTATION_OPERATOR].GetString();

        if(name == "do_nothing"){
            mutation = std::make_shared<DoNothingMinionMutation>();
        } else if(name == "custom"){
            mutation = std::make_shared<CustomMinionMutation>();
        } else {
            mutation = std::make_shared<DoNothingMinionMutation>();
        }
    } else {
        mutation = std::make_shared<DoNothingMinionMutation>();
    }

    if(root.HasMember(MUTATION_OPERATOR_CONFIG) && root[MUTATION_OPERATOR_CONFIG].IsObject()){
        mutation->configureFromJSON(root[MUTATION_OPERATOR_CONFIG]);
    }

    if(root.HasMember(FOOD_PELLETS) && root[FOOD_PELLETS].IsInt()){
        nFoodPellets = root[FOOD_PELLETS].GetInt();
        for(int i = 0; i < nFoodPellets; i++){
            auto pellet = std::make_shared<Pellet>(true);
            pellet->setColor(SimConst::FOOD_PELLET_COLOR);
            initializePellet(pellet);
            foodPellets.push_back(pellet);
        }
    }

    if(root.HasMember(POISON_PELLETS) && root[POISON_PELLETS].IsInt()){
        nPoisonPellets = root[POISON_PELLETS].GetInt();
        for(int i = 0; i < nPoisonPellets; i++){
            auto pellet = std::make_shared<Pellet>(false);
            pellet->setColor(SimConst::POISON_PELLET_COLOR);
            initializePellet(pellet);
            poisonPellets.push_back(pellet);
        }
    }

    if(root.HasMember(NUM_DEFAULT_MINIONS) && root[NUM_DEFAULT_MINIONS].IsInt()){
        nDefaultMinions = root[NUM_DEFAULT_MINIONS].GetInt();
    }
}

int State::getNMinions() const {
    return nMinions;
}

void State::setNMinions(int nMinions) {
    State::nMinions = nMinions;
}

void State::setFitnessAlg(std::shared_ptr<Fitness> fitness) {
    State::fitnessAlg = fitness;
}

void State::initializePellet(std::shared_ptr<Pellet> pellet) {
    std::uniform_real_distribution<float> angleDistr(0.f, glm::two_pi<float>());
    std::uniform_real_distribution<float> distanceDistr(0.f, boundary->getR1() - pellet->getRadius());

    float angle = angleDistr(RNG::get());
    float dist = distanceDistr(RNG::get());
    pellet->setPos(glm::vec2(dist * glm::cos(angle), dist * glm::sin(angle)));

    pellet->setLife(SimConst::PELLET_LIFE);
}

void State::resolveMinionFoodPelletDamage(Minion &m, Pellet &p, CollisionInfo &ci) {
    const float LEECH_CONSTANT = 0.75f;

    if(!m.isDead()){
        m.setLife(m.getLife() + LEECH_CONSTANT);
        m.setHealthRecovered(m.getHealthRecovered() + LEECH_CONSTANT);

        p.setLife(p.getLife() - LEECH_CONSTANT);
    }
}

void State::resolveMinionPoisonPelletDamage(Minion &m, Pellet &p, CollisionInfo &ci) {
    const float LEECH_CONSTANT = 0.5f;

    if(!m.isDead()){
        m.setLife(m.getLife() - LEECH_CONSTANT);

        p.setLife(p.getLife() - LEECH_CONSTANT);
    }
}

const std::vector< std::shared_ptr<Pellet> >& State::getFoodPellets() const {
    return foodPellets;
}

const std::vector< std::shared_ptr<Pellet> >& State::getPoisonPellets() const {
    return poisonPellets;
}

bool State::isDrawSenses() const {
    return drawSenses;
}

void State::setDrawSenses(bool drawSenses) {
    State::drawSenses = drawSenses;
}

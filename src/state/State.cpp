#include <random>
#include <chrono>
#include "State.h"
#include "Log.h"
#include "../rendering/Renderer.h"
#include "../constants/SimulationConstants.h"
#include "../constants/WindowConstants.h"
#include "../collision/CollisionChecker.h"
#include "../collision/CollisionCircle.h"

State& State::getInstance() {
    static State instance;
    return instance;
 }

State::State() {
    Log().Get(logDEBUG) << "Creating new state instance";
    shouldEndProgramFlag = false;
    lastUpdateTimePoint = std::chrono::high_resolution_clock::now();
    rng.seed(lastUpdateTimePoint.time_since_epoch().count());
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
        minion->setScale(10.);
        minion->setPos(glm::vec2(posDistr(rng), posDistr(rng)));
        minion->setColor(glm::vec4(colDistr(rng), colDistr(rng), colDistr(rng), 1.f));
        minion->setAngle(angleDistr(rng));
        minions.push_back(minion);
    }
}

void State::draw() {
    for(std::shared_ptr<Minion> m : minions){
        m->draw();
    }
}

void State::update() {
    auto now = std::chrono::high_resolution_clock::now();
    
    typedef std::chrono::duration<long, std::nano> nanos_t;
    nanos_t duration(std::chrono::duration_cast<nanos_t>(now - lastUpdateTimePoint));
    
    lastUpdateTimePoint = now;
    float deltaT = (float) duration.count() / 1e9;
    
    std::normal_distribution<float> distrAngleAcc(0.f, 200.f);
    std::normal_distribution<float> distrAcc(10.f, 20.f);
    for(auto iter = minions.begin(); iter != minions.end(); iter++){
        auto m = *iter;
        
        for(auto iter2 = iter + 1; iter2 != minions.end(); iter2++){
            auto m2 = *iter2;
            
            bool isColl = CollisionChecker::checkCollision(*(m->getCollBounds()), *(m2->getCollBounds()));
            if(isColl){
                glm::vec2 conn = glm::normalize(m->getPos() - m2->getPos());
                glm::vec2 v1 = m->getVelocity() * conn;
                glm::vec2 v2 = m->getVelocity() * (-conn);
                
                m->setVelocity(m->getVelocity() - v1 + v2 + conn);
                m2->setVelocity(m2->getVelocity() - v2 + v1 - conn);
            }
        }
        
        m->setAcceleration(distrAcc(rng));
        m->setAngleAcc(distrAngleAcc(rng));
        
        m->update(deltaT);
    }
}

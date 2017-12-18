#include <random>
#include <chrono>
#include "State.h"
#include "Log.h"
#include "../rendering/Renderer.h"
#include "../constants/SimulationConstants.h"
#include "../constants/WindowConstants.h"

State& State::getInstance() {
    static State instance;
    return instance;
 }

State::State() {
    Log().Get(logDEBUG) << "Creating new state instance";
    shouldEndProgramFlag = false;
    lastUpdateTimePoint = std::chrono::high_resolution_clock::now();
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
    std::default_random_engine rng;
    std::uniform_real_distribution<float> posDistr((float) -WindowConst::WINDOW_HEIGHT / 2., (float) WindowConst::WINDOW_HEIGHT / 2.);
    std::uniform_real_distribution<float> colDistr(0.2, 1.);
    nMinions = n;
    Renderer &r = Renderer::getInstance();
    for(int i = 0; i < n; i++){
        std::shared_ptr<Minion> minion = std::make_shared<Minion>(r.getModel(SimConst::MINION_MODEL_NAME));
        minion->setScale(10.);
        minion->setPos(glm::vec2(posDistr(rng), posDistr(rng)));
        minion->setColor(glm::vec4(colDistr(rng), colDistr(rng), colDistr(rng), 1.f));
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
    
    std::default_random_engine rng;
    std::normal_distribution<float> distr(0.0, 1.0);
    for(std::shared_ptr<Minion> m : minions){
        m->setVelocity(m->getVelocity() + deltaT * glm::vec2(distr(rng), distr(rng)));
        m->update(deltaT);
    }
}

#include <random>
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
    std::uniform_real_distribution<float> distr((float) -WindowConst::WINDOW_HEIGHT, (float) WindowConst::WINDOW_HEIGHT);
    nMinions = n;
    Renderer &r = Renderer::getInstance();
    for(int i = 0; i < n; i++){
        std::shared_ptr<Minion> minion = std::make_shared<Minion>(r.getModel(SimConst::MINION_MODEL_NAME));
        minion->setScale(10.f);
        minion->setPos(glm::vec2(distr(rng), distr(rng)));
        minions.push_back(minion);
    }
}

void State::drawMinions() {
    for(std::shared_ptr<Minion> m : minions){
        m->draw();
    }
}

void State::updateMinions(float deltaT) {
    for(std::shared_ptr<Minion> m : minions){
        m->update(deltaT);
    }
}

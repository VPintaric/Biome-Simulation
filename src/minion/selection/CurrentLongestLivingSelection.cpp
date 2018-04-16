#include <state/State.h>
#include "minion/selection/CurrentLongestLivingSelection.h"

CurrentLongestLivingSelection::CurrentLongestLivingSelection() = default;
CurrentLongestLivingSelection::~CurrentLongestLivingSelection() = default;

std::shared_ptr<Minion> CurrentLongestLivingSelection::getNewMinion() {
    auto& s = State::getInstance();
    auto& minions = s.getMinions();
    auto rng = s.getRng();

    float sum = 0.f;

    if(minions.empty()){
        return nullptr;
    } else if(minions.size() == 1){
        return minionGenerator->generateChild(minions[0], minions[0]);
    }

    int first = -1, second = -1;
    for(auto m : minions){
        sum += m->getTimeLived();
        m->setFitness(m->getTimeLived());
    }

    std::uniform_real_distribution<float> distr(0.f, sum);
    float roll = distr(rng.get());
    float runningSum = 0.f;
    for(int i = 0; i < minions.size(); i++){
        runningSum += minions[i]->getTimeLived();
        if(roll <= runningSum && first == -1){
            first = i;
            sum -= minions[i]->getTimeLived();
            break;
        }
    }
    if(first == -1){
        sum -= minions.back()->getTimeLived();
        first = static_cast<int>(minions.size() - 1);
    }

    distr = std::uniform_real_distribution<float>(0.f, sum);
    roll = distr(rng.get());
    runningSum = 0.f;
    for(int i = 0; i < minions.size(); i++){
        if(i == first){
            continue;
        }
        runningSum += minions[i]->getTimeLived();
        if(roll <= runningSum){
            second = i;
            break;
        }
    }
    if(second == -1){
        second = static_cast<int>(minions.size() - 1);
        if(first == second){
            second--;
        }
    }

    return minionGenerator->generateChild(minions[first], minions[second]);
}

void CurrentLongestLivingSelection::setMinionGenerator(std::shared_ptr<MinionGenerator> mg) {
    minionGenerator = mg;
}

float CurrentLongestLivingSelection::getFitness(std::shared_ptr<Minion> m) {
    return m->getTimeLived();
}

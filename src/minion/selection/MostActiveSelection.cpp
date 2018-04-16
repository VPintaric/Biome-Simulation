#include <random>
#include <state/State.h>
#include <state/Log.h>
#include "minion/selection/MostActiveSelection.h"

std::shared_ptr<Minion> MostActiveSelection::getNewMinion() {
    updateFitness();

    auto& s = State::getInstance();
    auto& minions = s.getMinions();
    auto rng = s.getRng();

    float sum = 0.f;

    if(minions.empty()){
        return nullptr;
    } else if(minions.size() == 1){
        return mg->generateChild(minions[0], minions[0]);
    }

    int first = -1, second = -1;
    for(float f : fitness){
        sum += f;
    }

    std::uniform_real_distribution<float> distr(0.f, sum);
    float roll = distr(rng.get());
    float runningSum = 0.f;
    for(int i = 0; i < minions.size(); i++){
        runningSum += fitness[i];
        if(roll <= runningSum && first == -1){
            first = i;
            sum -= fitness[i];
            break;
        }
    }
    if(first == -1){
        sum -= fitness.back();
        first = static_cast<int>(minions.size() - 1);
    }

    distr = std::uniform_real_distribution<float>(0.f, sum);
    roll = distr(rng.get());
    runningSum = 0.f;
    for(int i = 0; i < minions.size(); i++){
        if(i == first){
            continue;
        }
        runningSum += fitness[i];
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

    return mg->generateChild(minions[first], minions[second]);
}

void MostActiveSelection::updateFitness() {
    auto& s = State::getInstance();

    fitness.clear();

    for(auto m : s.getMinions()){
        float f = getFitness(m);
        fitness.push_back(f);
        m->setFitness(f);
    }
}

void MostActiveSelection::setMinionGenerator(std::shared_ptr<MinionGenerator> mg) {
    this->mg = mg;
}

float MostActiveSelection::getFitness(std::shared_ptr<Minion> m) {
    float TIME_LIVED_COEFF = 1.f;
    float DISTANCE_COEFF = 0.02f;
    float DAMAGE_COEFF = 1.f;
    float RECOVERED_COEFF = 1.5f;

    return TIME_LIVED_COEFF * m->getTimeLived() +
            DISTANCE_COEFF * m->getDistanceTraveled() +
            DAMAGE_COEFF * m->getDamageDealt() +
            RECOVERED_COEFF * m->getHealthRecovered();
}

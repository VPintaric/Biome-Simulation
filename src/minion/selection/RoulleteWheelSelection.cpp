#include <state/State.h>
#include <state/Log.h>
#include <helpers/RNG.h>
#include "minion/selection/RoulleteWheelSelection.h"

void RoulleteWheelSelection::configureFromJSON(rjs::Value &root) {
    // Nothing to configure
}

std::pair<std::shared_ptr<Minion>, std::shared_ptr<Minion>>
RoulleteWheelSelection::selectParents(std::vector<std::shared_ptr<Minion>> &minions) {
    if(minions.size() < 2){
        Log().Get(logWARNING) << "Can't select 2 different parents from customMinions vector, vector size is " << minions.size();
        return std::make_pair(nullptr, nullptr);
    }

    float fitnessSum = 0.f;

    for(auto m : minions){
        fitnessSum += m->getFitness();
    }

    std::uniform_real_distribution<float> distr(0.f, fitnessSum);
    float roll = distr(RNG::get());
    std::shared_ptr<Minion> m1, m2;
    float runningSum = 0.f;

    for(auto m : minions){
        runningSum += m->getFitness();
        if(roll <= runningSum){
            m1 = m;
            fitnessSum -= m1->getFitness();
            break;
        }
    }

    if(m1 == nullptr){
        m1 = minions.back();
        fitnessSum -= m1->getFitness();
    }

    distr = std::uniform_real_distribution<float>(0.f, fitnessSum);
    roll = distr(RNG::get());
    runningSum = 0.f;

    for(auto m : minions){
        runningSum += m->getFitness();
        if(roll <= runningSum && m != m1){
            m2 = m;
            break;
        }
    }

    if(m2 == nullptr){
        m2 = minions.back();
        if(m1 == m2){
            m2 = minions.front();
        }
    }

    return std::make_pair(m1, m2);
}

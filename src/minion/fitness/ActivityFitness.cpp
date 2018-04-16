//
// Created by wex on 4/16/18.
//

#include <helpers/MathHelpers.h>
#include "minion/fitness/ActivityFitness.h"

ActivityFitness::ActivityFitness() : timeLivedCoeff(1.f), damageDealtCoeff(1.f),
                                     distanceTraveledCoeff(1.f), healthRecoveredCoeff(1.f) {

}

void ActivityFitness::configureFromJSON(rjs::Value &root) {
    const char * TIME_LIVED = "time_lived_coeff";
    const char * DAMAGE_DEALT = "damage_dealt_coeff";
    const char * HEALTH_RECOVERED = "health_recovered_coeff";
    const char * DISTANCE_TRAVELED = "distance_traveled_coeff";

    if(root.HasMember(TIME_LIVED) && root[TIME_LIVED].IsFloat()){
        timeLivedCoeff = root[TIME_LIVED].GetFloat();
    }

    if(root.HasMember(DAMAGE_DEALT) && root[DAMAGE_DEALT].IsFloat()){
        damageDealtCoeff = root[DAMAGE_DEALT].GetFloat();
    }

    if(root.HasMember(HEALTH_RECOVERED) && root[HEALTH_RECOVERED].IsFloat()){
        healthRecoveredCoeff = root[HEALTH_RECOVERED].GetFloat();
    }

    if(root.HasMember(DISTANCE_TRAVELED) && root[DISTANCE_TRAVELED].IsFloat()){
        distanceTraveledCoeff = root[DISTANCE_TRAVELED].GetFloat();
    }
}

void ActivityFitness::calculateFitness(std::shared_ptr<Minion> m) {
    m->setFitness(timeLivedCoeff * m->getTimeLived() + damageDealtCoeff * m->getDamageDealt() +
                    healthRecoveredCoeff * m->getHealthRecovered() + distanceTraveledCoeff * m->getDistanceTraveled());
}

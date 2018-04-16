#ifndef BIOME_SIMULATION_ACTIVITYFITNESS_H
#define BIOME_SIMULATION_ACTIVITYFITNESS_H


#include "Fitness.h"

class ActivityFitness : public Fitness {
private:
    float timeLivedCoeff, damageDealtCoeff, healthRecoveredCoeff, distanceTraveledCoeff;

public:
    ActivityFitness();

    void configureFromJSON(rjs::Value &root) override;

    void calculateFitness(std::shared_ptr<Minion> m) override;
};


#endif //BIOME_SIMULATION_ACTIVITYFITNESS_H

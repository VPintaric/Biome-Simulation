#ifndef SIMULATIONCONSTANTS_H
#define SIMULATIONCONSTANTS_H

#include <string>

namespace SimConst {
    const std::string MINION_BODY_MODEL_NAME = "minion_body_model";
    const std::string MINION_CIRCLE_MODEL_NAME = "minion_circle_model";
    const std::string HOLLOW_CIRCLE_MODEL_NAME = "hollow_circle_model";

    const float UPDATE_DELTA = 1e-3;
    const int UPDATE_ITERATIONS_EACH_FRAME = 10;

    const float MINION_MIN_RADIUS = 10.f;
    const float MINION_MAX_RADIUS = 30.f;
    const float MINION_RADIUS_TO_RMASS_RATIO = 20.f;
    const float MINION_MAX_LIFE_RADIUS_FACTOR = 2.5f;
    const float MINION_DEFAULT_DECAY_RATE = 1.0f;
    const float MINION_BODY_DECAY_RATE = 1.0f;
    const float MINION_DECAY_RATE_FORCE_FACTOR = 1e-4f;
    const float MINION_DECAY_RATE_MOMENT_FACTOR = 1e-4f;
    const float MINION_MAX_MAX_SENSE_DISTANCE = 100.f;
    const float MINION_MIN_MAX_SENSE_DISTANCE = 40.f;
    const float MINION_DECAY_RATE_SENSE_DISTANCE_FACTOR = 0.01f;
    const float MINION_DEFAULT_MAX_SENSE_DISTANCE = 70.f;
    const int MINION_DEFAULT_N_SIGHT_LINES = 15;

    const float DEFAULT_BOUNDARY_RADIUS = 500.f;
    const int DEFAULT_NUMBER_OF_MINIONS = 30;
}

#endif /* SIMULATIONCONSTANTS_H */


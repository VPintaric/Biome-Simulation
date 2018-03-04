#ifndef SIMULATIONCONSTANTS_H
#define SIMULATIONCONSTANTS_H

#include <string>

namespace SimConst {
    const std::string MINION_BODY_MODEL_NAME = "minion_body_model";
    const std::string MINION_CIRCLE_MODEL_NAME = "minion_circle_model";
    const std::string HOLLOW_CIRCLE_MODEL_NAME = "hollow_circle_model";

    const float UPDATE_DELTA = 1e-3;
    const int UPDATE_ITERATIONS_EACH_FRAME = 10;
    
    const float MINION_MAX_VELOCITY = 20.f;
    const float MINION_MIN_SCALE = 3.f;
    const float MINION_MAX_SCALE = 25.f;
    const float MINION_MIN_MASS = 0.1f;
    const float MINION_MAX_MASS = 50.f;
    const float MINION_MIN_ACC = -10.f;
    const float MINION_MAX_ACC = 20.f;
    const float MINION_MAX_ANGLE_ACC = 300.f;
    const float MINION_MIN_ANGLE_ACC = -MINION_MAX_ANGLE_ACC;
}

#endif /* SIMULATIONCONSTANTS_H */


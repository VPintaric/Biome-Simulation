#ifndef SIMULATIONCONSTANTS_H
#define SIMULATIONCONSTANTS_H

#include <string>

namespace SimConst {
    const std::string MINION_MODEL_NAME = "minion_model";
    
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


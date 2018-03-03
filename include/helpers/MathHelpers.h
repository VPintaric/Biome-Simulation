
#ifndef MATH_HELPERS_H
#define MATH_HELPERS_H

#include "glm/glm.hpp"

namespace Math{
    float clamp(const float& x, const float& lo, const float& hi);
    float sign(const float& x);

    glm::vec2 perp(glm::vec2 &v);
    float cross(glm::vec2 &v1, glm::vec2 &v2);

    float sigmoid(float x);
    float relu(float x);
    float fastSigmoid(float x);
}

#endif // MATH_HELPERS_H
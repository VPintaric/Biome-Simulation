
#ifndef MATH_HELPERS_H
#define MATH_HELPERS_H

#include <GL/glm/glm.hpp>

namespace Math{
    float clamp(const float& x, const float& lo, const float& hi);
    float sign(const float& x);

    glm::vec2 perp(glm::vec2 &v);
    float cross(glm::vec2 &v1, glm::vec2 &v2);
}

#endif // MATH_HELPERS_H
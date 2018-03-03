#include "helpers/MathHelpers.h"

namespace Math{
    float clamp(const float &x, const float &lo, const float &hi) {
        return  x < lo ? lo : (x > hi ? hi : x);
    }

    float sign(const float& x){
        return x < 0.f ? -1.f : (x > 0.f ? 1.f : 0.f);
    }

    float cross(glm::vec2 &v1, glm::vec2 &v2) {
        return v1.x * v2.y - v2.x * v1.y;
    }

    glm::vec2 perp(glm::vec2 &v) {
        return glm::vec2(-v.y, v.x);
    }

    float sigmoid(float x){
        return (1.f / (1.f + expf(-x)));
    }

    float relu(float x){
        return x < 0.f ? 0.f : x;
    }

    float fastSigmoid(float x){
        return x / (1.f + abs(x));
    }
}

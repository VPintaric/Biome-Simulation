
#include <GL/glm/glm.hpp>

namespace Math{
    template <typename T>
    T clamp(const T& x, const T& lo, const T& hi){
        return  x < lo ? lo : (x > hi ? hi : x);
    }

    glm::vec2 perp(glm::vec2 &v){
        return glm::vec2(-v.y, v.x);
    }

    float cross(glm::vec2 &v1, glm::vec2 &v2){
        return v1.x * v2.y - v2.x * v1.y;
    }
}
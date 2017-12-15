#include "MathHelpers.h"

template <typename T>
T Math::clamp(const T& x, const T& lo, const T& hi){
    return  x < lo ? lo : (x > hi ? hi : x);
}
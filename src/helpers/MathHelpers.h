
namespace Math{
    template <typename T>
    T clamp(const T& x, const T& lo, const T& hi){
        return  x < lo ? lo : (x > hi ? hi : x);
    }
}
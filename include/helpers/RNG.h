#ifndef BIOME_SIMULATION_RNG_H
#define BIOME_SIMULATION_RNG_H

#include <random>
#include <memory>

class RNG {
    std::default_random_engine rng;
    static std::unique_ptr<RNG> instance;

    RNG();

    void _seed(unsigned long seed);

public:

    static void seed(unsigned long seed);
    static std::default_random_engine& get();
    static float roll();
    ~RNG();
};


#endif //BIOME_SIMULATION_RNG_H

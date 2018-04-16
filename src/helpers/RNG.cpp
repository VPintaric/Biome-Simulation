#include <chrono>
#include "helpers/RNG.h"

RNG::RNG() = default;
RNG::~RNG() = default;

std::unique_ptr<RNG> RNG::instance;

std::default_random_engine& RNG::get() {
    if(instance == nullptr){
        instance = std::unique_ptr<RNG>(new RNG);
        instance->_seed(static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
    }
    return instance->rng;
}

void RNG::_seed(unsigned long seed) {
    rng.seed(seed);
}

void RNG::seed(unsigned long seed) {
    if(instance == nullptr){
        instance = std::unique_ptr<RNG>(new RNG);
    }
    instance->_seed(seed);
}
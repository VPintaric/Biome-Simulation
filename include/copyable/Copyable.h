#ifndef BIOME_SIMULATION_COPYABLE_H
#define BIOME_SIMULATION_COPYABLE_H

#include <memory>

template<class T>
class Copyable {
public:
    virtual std::shared_ptr<T> copy() = 0;
};

#endif //BIOME_SIMULATION_COPYABLE_H

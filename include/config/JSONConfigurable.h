#ifndef BIOME_SIMULATION_JSONCONFIGURABLE_H
#define BIOME_SIMULATION_JSONCONFIGURABLE_H

#include <rapidjson/document.h>

namespace rjs = rapidjson;

class JSONConfigurable {
public:
    JSONConfigurable() = default;
    virtual ~JSONConfigurable() = default;

    virtual void configureFromJSON(rjs::Value& root) = 0;
};

#endif //BIOME_SIMULATION_JSONCONFIGURABLE_H

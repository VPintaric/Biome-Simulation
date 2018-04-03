#ifndef BIOME_SIMULATION_JSONPERSISTABLE_H
#define BIOME_SIMULATION_JSONPERSISTABLE_H

#include <rapidjson/document.h>
#include <memory>

namespace rjs = rapidjson;

class JSONPersistable {
public:
    virtual void persistToJSON(rjs::Value &root, rjs::Document::AllocatorType& alloc) = 0;
    virtual void initFromJSON(rjs::Value &root) = 0;
};


#endif //BIOME_SIMULATION_JSONPERSISTABLE_H

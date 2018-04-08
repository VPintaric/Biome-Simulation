#ifndef BIOME_SIMULATION_CONFIG_H
#define BIOME_SIMULATION_CONFIG_H

#include <string>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

namespace rjs = rapidjson;

class Config {
public:
    static Config& getInstance();

    virtual ~Config();

    void parseConfigFile(std::string fileName);

    rjs::Document& getConfig();

private:

    Config();

    rjs::Document doc;
};


#endif //BIOME_SIMULATION_CONFIG_H

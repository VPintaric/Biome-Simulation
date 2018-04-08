#include <state/Log.h>
#include <rapidjson/filereadstream.h>
#include "config/Config.h"

Config::Config() = default;

Config::~Config() = default;

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

rjs::Document& Config::getConfig() {
    return doc;
}

void Config::parseConfigFile(std::string fileName) {
    FILE* fp = fopen(fileName.c_str(), "rb");

    if(!fp){
        Log().Get(logWARNING) << "Can't read config file, can't open file: " << fileName;
        doc.Parse("{}"); // parse an empty JSON object to evade some errors later
        return;
    }

    char buffer[65536];
    rjs::FileReadStream fs(fp, buffer, sizeof(buffer));

    doc.ParseStream(fs);

    if(!doc.IsObject()){
        Log().Get(logWARNING) << "Root value in config file is not an object";
        doc.Parse("{}");
    }

    fclose(fp);
}
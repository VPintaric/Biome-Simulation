#include "persistence/Persistence.h"
#include <rapidjson/writer.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/filereadstream.h>
#include <state/Log.h>

namespace rjs = rapidjson;

Persistence::Persistence() = default;
Persistence::~Persistence() = default;

Persistence& Persistence::getInstance() {
    static Persistence instance;
    return instance;
}

void Persistence::saveMinionToFile(std::string fileName, std::shared_ptr<Minion> m) {
    FILE* fp = fopen(fileName.c_str(), "wb");

    if(!fp){
        Log().Get(logWARNING) << "Can't persist minion to file, can't open file: " << fileName;
        return;
    }

    char buffer[65536];
    rjs::FileWriteStream fs(fp, buffer, sizeof(buffer));
    rjs::Writer<rjs::FileWriteStream> writer(fs);

    rjs::Document doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();

    m->persistToJSON(doc, alloc);

    doc.Accept(writer);
    fclose(fp);
}

void Persistence::initMinionFromFile(std::string fileName, std::shared_ptr<Minion> m) {
    FILE* fp = fopen(fileName.c_str(), "rb");

    if(!fp){
        Log().Get(logWARNING) << "Can't initialize minion from file, can't open file: " << fileName;
        return;
    }

    char buffer[65536];
    rjs::FileReadStream fs(fp, buffer, sizeof(buffer));

    rjs::Document doc;
    doc.ParseStream(fs);

    m->initFromJSON(doc);

    fclose(fp);
}
#ifndef BIOME_SIMULATION_PERSISTANCE_H
#define BIOME_SIMULATION_PERSISTANCE_H


#include <minion/Minion.h>
#include <string>

class Persistence {
public:
    virtual ~Persistence();
    static Persistence& getInstance();

    void saveMinionToFile(std::string fileName, std::shared_ptr<Minion> m);
    void initMinionFromFile(std::string fileName, std::shared_ptr<Minion> m);

private:
    Persistence();
};


#endif //BIOME_SIMULATION_PERSISTANCE_H

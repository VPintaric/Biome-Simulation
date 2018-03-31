#ifndef BIOME_SIMULATION_SELECTION_H
#define BIOME_SIMULATION_SELECTION_H


#include <minion/Minion.h>
#include <minion/factories/MinionGenerator.h>

class Selection {
public:
    Selection();
    virtual ~Selection();

    virtual std::shared_ptr<Minion> getNewMinion() = 0;
    virtual void setMinionGenerator(std::shared_ptr<MinionGenerator> mg) = 0;
};


#endif //BIOME_SIMULATION_SELECTION_H

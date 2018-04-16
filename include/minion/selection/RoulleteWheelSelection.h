#ifndef BIOME_SIMULATION_ROULLETEWHEELSELECTION_H
#define BIOME_SIMULATION_ROULLETEWHEELSELECTION_H


#include "Selection.h"

class RoulleteWheelSelection : public Selection {

public:
    void configureFromJSON(rjs::Value &root) override;

    std::pair<std::shared_ptr<Minion>, std::shared_ptr<Minion>>
        selectParents(std::vector<std::shared_ptr<Minion>> &minions) override;

};


#endif //BIOME_SIMULATION_ROULLETEWHEELSELECTION_H

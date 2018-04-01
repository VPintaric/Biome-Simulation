#ifndef BIOME_SIMULATION_CURRENTLONGESTLIVINGSELECTION_H
#define BIOME_SIMULATION_CURRENTLONGESTLIVINGSELECTION_H


#include "Selection.h"

class CurrentLongestLivingSelection : public Selection {
public:
    CurrentLongestLivingSelection();
    ~CurrentLongestLivingSelection();

    std::shared_ptr<Minion> getNewMinion() override;
    void setMinionGenerator(std::shared_ptr<MinionGenerator> mg) override;

private:
    std::shared_ptr<MinionGenerator> minionGenerator;
};


#endif //BIOME_SIMULATION_CURRENTLONGESTLIVINGSELECTION_H

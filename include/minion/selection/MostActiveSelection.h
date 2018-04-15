#ifndef BIOME_SIMULATION_MOSTACTIVESELECTION_H
#define BIOME_SIMULATION_MOSTACTIVESELECTION_H


#include <minion/selection/Selection.h>

class MostActiveSelection : public Selection {
private:
    std::vector<float> fitness;
    std::shared_ptr<MinionGenerator> mg;

    void updateFitness();

public:
    std::shared_ptr<Minion> getNewMinion() override;

    void setMinionGenerator(std::shared_ptr<MinionGenerator> mg) override;

    float getFitness(std::shared_ptr<Minion> m) override;
};


#endif //BIOME_SIMULATION_MOSTACTIVESELECTION_H

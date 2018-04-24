#ifndef BIOME_SIMULATION_DECISIONTREECONTROLLERCROSSOVER_H
#define BIOME_SIMULATION_DECISIONTREECONTROLLERCROSSOVER_H

#include <minion/crossover_operators/controller/MinionControllerCrossover.h>
#include <minion/controllers/decision_tree/DecisionTreeController.h>

class DecisionTreeControllerCrossover : public MinionControllerCrossover {
public:
    std::shared_ptr<MinionController>
    crossover(std::shared_ptr<MinionController> p1, std::shared_ptr<MinionController> p2, float f1, float f2) override;

    virtual std::shared_ptr<DecisionTreeController> decisionTreeCrossover(std::shared_ptr<DecisionTreeController> p1,
                                                                          std::shared_ptr<DecisionTreeController> p2,
                                                                          float f1, float f2) = 0;
};


#endif //BIOME_SIMULATION_DECISIONTREECONTROLLERCROSSOVER_H

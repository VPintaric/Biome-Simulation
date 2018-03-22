#ifndef BIOME_SIMULATION_NEURALNETCONTROLLER_H
#define BIOME_SIMULATION_NEURALNETCONTROLLER_H


#include "MinionController.h"
#include "eigen/Eigen"
#include "neuralnet/NeuralNet.h"

#include <vector>

class NeuralNetController : public MinionController {
private:
    const int INPUT_VARS = 3;
    const int OUTPUT_VARS = 2;

    std::shared_ptr<NeuralNet> nn;

public:
    explicit NeuralNetController(const std::vector<int> &hiddenLayers, std::function<float(float)> activation);
    ~NeuralNetController() override;
    void controlMinion(const std::shared_ptr<MinionObject> &m, const std::shared_ptr<MinionSenses> &senses) override;
    std::shared_ptr<NeuralNet> getNeuralNet();
};


#endif //BIOME_SIMULATION_NEURALNETCONTROLLER_H

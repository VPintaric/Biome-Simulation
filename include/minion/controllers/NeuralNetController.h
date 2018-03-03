#ifndef BIOME_SIMULATION_NEURALNETCONTROLLER_H
#define BIOME_SIMULATION_NEURALNETCONTROLLER_H


#include "MinionController.h"
#include "eigen/Eigen"

#include <vector>

class NeuralNetController : public MinionController {
private:
    const int INPUT_VARS = 3;
    const int OUTPUT_VARS = 2;

    std::vector<std::shared_ptr<Eigen::MatrixXf> > weights;
    std::vector<std::shared_ptr<Eigen::MatrixXf> > bias;
    std::function<float(float)> activation;

public:
    explicit NeuralNetController(const std::vector<int> &hiddenLayers, std::function<float(float)> activation);
    ~NeuralNetController() override;
    void controlMinion(const std::shared_ptr<MinionObject> &m, const std::shared_ptr<MinionSenses> &senses) override;
};


#endif //BIOME_SIMULATION_NEURALNETCONTROLLER_H

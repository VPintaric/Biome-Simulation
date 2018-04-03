#ifndef BIOME_SIMULATION_NEURALNETCONTROLLER_H
#define BIOME_SIMULATION_NEURALNETCONTROLLER_H


#include "MinionController.h"
#include "eigen/Eigen"
#include "neuralnet/NeuralNet.h"

#include <vector>

class NeuralNetController : public MinionController {
public:
    explicit NeuralNetController(std::weak_ptr<Minion> minion,
                                 const std::vector<int> &hiddenLayers,
                                 std::function<float(float)> activation);
    ~NeuralNetController() override;

    std::vector<float> controlMinion(std::vector<float> senseData) override;

    std::shared_ptr<NeuralNet> getNeuralNet();

    void persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) override;

    void initFromJSON(rjs::Value &root) override;

private:

    const int OUTPUT_VARS = 2;

    std::shared_ptr<NeuralNet> nn;

    std::weak_ptr<Minion> minion;

    const char * JSON_NEURAL_NET = "neural_net";
};


#endif //BIOME_SIMULATION_NEURALNETCONTROLLER_H

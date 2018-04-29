#ifndef BIOME_SIMULATION_NEURALNETCONTROLLER_H
#define BIOME_SIMULATION_NEURALNETCONTROLLER_H


#include "minion/controllers/MinionController.h"
#include "eigen/Eigen"
#include "NeuralNet.h"

#include <vector>

class NeuralNetController : public MinionController {
public:
    explicit NeuralNetController(int inputDataSize,
                                 const std::vector<int> &hiddenLayers,
                                 std::function<float(float)> activation);
    ~NeuralNetController() override;

    std::vector<float> controlMinion(std::vector<float> senseData) override;

    std::shared_ptr<NeuralNet> getAccNeuralNet();

    std::shared_ptr<NeuralNet> getRotNeuralNet();

    void persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) override;

    void initFromJSON(rjs::Value &root) override;

    std::shared_ptr<MinionController> copy() override;

    void setMinion(std::shared_ptr<Minion> m) override;

private:

    NeuralNetController();

    std::shared_ptr<NeuralNet> accNN, rotNN;

    int inputDataSize;

    const char * JSON_ACC_NEURAL_NET = "acceleration_neural_net";
    const char * JSON_ROT_NEURAL_NET = "rotation_neural_net";

    std::shared_ptr<Minion> minion;
};


#endif //BIOME_SIMULATION_NEURALNETCONTROLLER_H

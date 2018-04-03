#ifndef BIOME_SIMULATION_NEURALNET_H
#define BIOME_SIMULATION_NEURALNET_H


#include <functional>
#include <eigen/Eigen>
#include <memory>
#include <string>

class NeuralNet : public JSONPersistable {
public:
    std::vector<int> layers;
    std::vector<std::shared_ptr<Eigen::MatrixXf> > weights;
    std::vector<std::shared_ptr<Eigen::MatrixXf> > bias;

    std::function<float(float)> activation;

    NeuralNet(const std::vector<int> &layers, std::function<float(float)> activation);

    ~NeuralNet();

    Eigen::MatrixXf forward(Eigen::MatrixXf input);

    void initRandom();

    void persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) override;

    void initFromJSON(rjs::Value &root) override;

private:
    const char * JSON_LAYERS = "layers";
    const char * JSON_WEIGHTS = "weights";
    const char * JSON_BIAS = "bias";
};


#endif //BIOME_SIMULATION_NEURALNET_H

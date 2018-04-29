#ifndef BIOME_SIMULATION_NEURALNET_H
#define BIOME_SIMULATION_NEURALNET_H


#include <functional>
#include <eigen/Eigen>
#include <memory>
#include <string>
#include <rapidjson/rapidjson.h>
#include <copyable/Copyable.h>
#include "persistence/JSONPersistable.h"

namespace rjs = rapidjson;

class NeuralNet : public JSONPersistable, public Copyable<NeuralNet> {
public:
    std::vector<int> layers;
    std::vector<std::shared_ptr<Eigen::MatrixXf> > weights;
    std::vector<std::shared_ptr<Eigen::MatrixXf> > bias;

    const float WEIGHT_LIMIT = 20.f;
    const float BIAS_LIMIT = 50.f;

    std::function<float(float)> activation;

    NeuralNet(const std::vector<int> &layers, std::function<float(float)> activation);

    ~NeuralNet();

    Eigen::MatrixXf forward(Eigen::MatrixXf input);

    void initRandom();

    void persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) override;

    void initFromJSON(rjs::Value &root) override;

    std::shared_ptr<NeuralNet> copy() override;

    void clampWeights();

private:
    const char * JSON_LAYERS = "layers";
    const char * JSON_WEIGHTS = "weights";

    const char * JSON_BIAS = "bias";
};


#endif //BIOME_SIMULATION_NEURALNET_H

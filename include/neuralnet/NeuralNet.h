#ifndef BIOME_SIMULATION_NEURALNET_H
#define BIOME_SIMULATION_NEURALNET_H


#include <functional>
#include <eigen/Eigen>
#include <memory>
#include <string>

class NeuralNet {
private:
public:
    std::vector<std::shared_ptr<Eigen::MatrixXf> > weights;
    std::vector<std::shared_ptr<Eigen::MatrixXf> > bias;

    std::function<float(float)> activation;

    NeuralNet(const std::vector<int> &layers, std::function<float(float)> activation);
    explicit NeuralNet(std::string fileName);
    ~NeuralNet();

    Eigen::MatrixXf forward(Eigen::MatrixXf input);
    void saveToFile(std::string fileName);
    void loadFromFile(std::string fileName);
    void initRandom();
};


#endif //BIOME_SIMULATION_NEURALNET_H

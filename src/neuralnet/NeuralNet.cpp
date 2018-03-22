#include <fstream>
#include <utility>
#include <state/Log.h>
#include <state/State.h>
#include "neuralnet/NeuralNet.h"

NeuralNet::NeuralNet(const std::vector<int> &layers, std::function<float(float)> activation) :
                    activation(std::move(activation)){
    int in = layers.front();
    for (auto iter = layers.begin() + 1; iter != layers.end() - 1; ++iter) {
        auto w = std::make_shared<Eigen::MatrixXf>();
        w->setZero(in, *iter);
        auto b = std::make_shared<Eigen::MatrixXf>();
        b->setZero(1, *iter);
        weights.push_back(w);
        bias.push_back(b);
        in = *iter;
    }

    auto w = std::make_shared<Eigen::MatrixXf>();
    w->setZero(in, layers.back());
    auto b = std::make_shared<Eigen::MatrixXf>();
    b->setZero(1, layers.back());
    weights.push_back(w);
    bias.push_back(b);
}

NeuralNet::NeuralNet(std::string fileName) {
    this->loadFromFile(std::move(fileName));
}

NeuralNet::~NeuralNet() = default;

Eigen::MatrixXf NeuralNet::forward(Eigen::MatrixXf input) {

    for(int i = 0; i < weights.size(); i++){
        auto w = weights[i];
        auto b = bias[i];
        input *= *w;
        input += *b;

        if(i < weights.size() - 1){
            input = input.unaryExpr(activation);
        }
    }

    return input;
}

/*
 * Saving it in plain text format for easier debugging and human readability.
 * Could be optimized by doing this in a separate thread and/or saving in binary format.
 */
void NeuralNet::saveToFile(std::string fileName) {
    std::ofstream file;
    file.open(fileName);

    if(!file.is_open()){
        Log().Get(logWARNING) << "Unable to save neuralnet data to file named: " << fileName;
        return;
    }

    for(int i = 0; i < weights.size(); ++i){
        auto& w = *weights[i];
        auto& b = *bias[i];

        file << w.rows() << " " << w.cols() << std::endl;

        for(int j = 0; j < w.rows(); j++){
            for(int k = 0; k < w.cols(); k++){
                file << w(j, k) << " ";
            }
        }
        file << std::endl;

        for(int j = 0; j < b.cols(); j++){
            file << b(0, j) << " ";
        }
        file << std::endl << std::endl;
    }

    file.close();
}

void NeuralNet::loadFromFile(std::string fileName) {
    std::ifstream file;
    file.open(fileName);

    if(!file.is_open()){
        Log().Get(logWARNING) << "Unable to load neuralnet from file named: " << fileName;
        return;
    }

    std::string line;
    while(std::getline(file, line)){
        if(line.empty()){
            continue;
        }

        std::istringstream iss(line);

        int rows, cols;
        iss >> rows >> cols;

        auto w = std::make_shared<Eigen::MatrixXf>();
        w->setZero(rows, cols);
        auto b = std::make_shared<Eigen::MatrixXf>();
        b->setZero(1, cols);

        std::getline(file, line);
        iss.str(line);
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                iss >> w->operator()(i, j);
            }
        }

        std::getline(file, line);
        iss.str(line);
        for(int j = 0; j < cols; j++){
            iss >> b->operator()(0, j);
        }

        weights.push_back(w);
        bias.push_back(b);
    }
}

void NeuralNet::initRandom() {
    std::uniform_real_distribution<float> distr(-100.f, 100.f);

    State& s = State::getInstance();
    auto rng = s.getRng();

    for(int layer = 0; layer < weights.size(); layer++){
        for(int i = 0; i < weights[layer]->rows(); i++){
            for(int j = 0; j < weights[layer]->cols(); j++){
                weights[layer]->operator()(i, j) = distr(rng.get());
                bias[layer]->operator()(0, j) = distr(rng.get());
            }
        }
    }
}
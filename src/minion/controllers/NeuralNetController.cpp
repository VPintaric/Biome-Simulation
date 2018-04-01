#include <memory>
#include <utility>
#include <state/State.h>
#include <state/Log.h>

#include "minion/controllers/NeuralNetController.h"

NeuralNetController::NeuralNetController(std::weak_ptr<Minion> minion,
                                         const std::vector<int> &hiddenLayers = std::vector<int>(),
                                         std::function<float(float)> activation = tanhf)
                                        :  minion(minion) {
    auto m = minion.lock();

    std::vector<int> layers;
    layers.reserve(hiddenLayers.size() + 2);
    layers.push_back(m->getSenses()->getDataSize());
    layers.insert(layers.end(), hiddenLayers.begin(), hiddenLayers.end());
    layers.push_back(OUTPUT_VARS);

    nn = std::make_shared<NeuralNet>(layers, activation);
};

NeuralNetController::~NeuralNetController() = default;

std::shared_ptr<NeuralNet> NeuralNetController::getNeuralNet() {
    return nn;
}

std::vector<float> NeuralNetController::controlMinion(std::vector<float> senseData) {
    Eigen::MatrixXf x(1, senseData.size());

    for(int i = 0; i < senseData.size(); i++){
        x(0, i) = senseData[i];
    }

    x = nn->forward(x);

    return std::vector<float>({x(0, 0), x(0, 1)});
}

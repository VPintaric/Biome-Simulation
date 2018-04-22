#include <memory>
#include <state/State.h>
#include <state/Log.h>

#include "minion/controllers/neural_net/NeuralNetController.h"

NeuralNetController::NeuralNetController() = default;

NeuralNetController::NeuralNetController(int inputDataSize,
                                         const std::vector<int> &hiddenLayers = std::vector<int>(),
                                         std::function<float(float)> activation = tanhf)
                                        :  inputDataSize(inputDataSize) {
    std::vector<int> layers;
    layers.reserve(hiddenLayers.size() + 2);
    layers.push_back(inputDataSize);
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

void NeuralNetController::persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) {
    root.SetObject();

    rjs::Value nnJSON(rjs::kObjectType);

    nn->persistToJSON(nnJSON, alloc);

    root.AddMember(rjs::StringRef(JSON_NEURAL_NET), nnJSON, alloc);
}

void NeuralNetController::initFromJSON(rjs::Value &root) {
    auto neuralnet = root.FindMember(rjs::StringRef(JSON_NEURAL_NET));
    if(neuralnet == root.MemberEnd() || !neuralnet->value.IsObject()){
        Log().Get(logWARNING) << "\"" << JSON_NEURAL_NET << "\" not found or invalid in JSON, can't initalize";
    } else {
        nn->initFromJSON(neuralnet->value);
    }
}

std::shared_ptr<MinionController> NeuralNetController::copy() {
    auto newCopy = std::shared_ptr<NeuralNetController>(new NeuralNetController);

    newCopy->inputDataSize = inputDataSize;
    newCopy->nn = nn->copy();

    return newCopy;
}

void NeuralNetController::setMinion(std::shared_ptr<Minion> m) {
    // This controller doesn't need a reference to minion it's controlling
}

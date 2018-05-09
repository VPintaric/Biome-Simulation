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
    layers.push_back(1);

    accNN = std::make_shared<NeuralNet>(layers, activation);
    rotNN = std::make_shared<NeuralNet>(layers, activation);
};

NeuralNetController::~NeuralNetController() = default;

std::shared_ptr<NeuralNet> NeuralNetController::getAccNeuralNet() {
    return accNN;
}

std::shared_ptr<NeuralNet> NeuralNetController::getRotNeuralNet() {
    return rotNN;
}

std::vector<float> NeuralNetController::controlMinion(std::vector<float> senseData) {
    minion->getSenses()->standardizeData(senseData);

    Eigen::MatrixXf x(1, senseData.size());
    for(int i = 0; i < senseData.size(); i++){
        x(0, i) = senseData[i];
    }
    auto acc = accNN->forward(x);
    auto rot = rotNN->forward(x);

    return std::vector<float>({400.f * acc(0, 0), 1500.f * rot(0, 0)}); // TODO: are these values good as scaling coeffs?
}

void NeuralNetController::persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) {
    root.SetObject();

    rjs::Value accNNJSON(rjs::kObjectType);
    accNN->persistToJSON(accNNJSON, alloc);
    root.AddMember(rjs::StringRef(JSON_ACC_NEURAL_NET), accNNJSON, alloc);

    rjs::Value rotNNJSON(rjs::kObjectType);
    rotNN->persistToJSON(rotNNJSON, alloc);
    root.AddMember(rjs::StringRef(JSON_ROT_NEURAL_NET), rotNNJSON, alloc);
}

void NeuralNetController::initFromJSON(rjs::Value &root) {
    auto accNNJSON = root.FindMember(JSON_ACC_NEURAL_NET);
    if(accNNJSON == root.MemberEnd() || !accNNJSON->value.IsObject()){
        Log().Get(logWARNING) << "\"" << JSON_ACC_NEURAL_NET << "\" not found or invalid in JSON, can't initalize";
    } else {
        accNN->initFromJSON(accNNJSON->value);
    }

    auto rotNNJSON = root.FindMember(JSON_ROT_NEURAL_NET);
    if(rotNNJSON == root.MemberEnd() || !rotNNJSON->value.IsObject()){
        Log().Get(logWARNING) << "\"" << JSON_ROT_NEURAL_NET << "\" not found or invalid in JSON, can't initalize";
    } else {
        rotNN->initFromJSON(rotNNJSON->value);
    }
}

std::shared_ptr<MinionController> NeuralNetController::copy() {
    auto newCopy = std::shared_ptr<NeuralNetController>(new NeuralNetController);

    newCopy->inputDataSize = inputDataSize;
    newCopy->accNN = accNN->copy();
    newCopy->rotNN = rotNN->copy();

    return newCopy;
}

void NeuralNetController::setMinion(std::shared_ptr<Minion> m) {
    minion = m;
}
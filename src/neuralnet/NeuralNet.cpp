#include <fstream>
#include <utility>
#include <state/Log.h>
#include <state/State.h>
#include <helpers/RNG.h>
#include "neuralnet/NeuralNet.h"

NeuralNet::NeuralNet(const std::vector<int> &layers, std::function<float(float)> activation) :
                    activation(std::move(activation)), layers(layers), Copyable<NeuralNet>() {
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

void NeuralNet::initRandom() {
    std::uniform_real_distribution<float> distr(-100.f, 100.f);

    for(int layer = 0; layer < weights.size(); layer++){
        for(int i = 0; i < weights[layer]->rows(); i++){
            for(int j = 0; j < weights[layer]->cols(); j++){
                weights[layer]->operator()(i, j) = distr(RNG::get());
                bias[layer]->operator()(0, j) = distr(RNG::get());
            }
        }
    }
}

void NeuralNet::persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) {
    root.SetObject();

    rjs::Value layersJSON(rjs::kArrayType);
    for(int layer : layers){
        layersJSON.PushBack(rjs::Value(layer), alloc);
    }
    root.AddMember(rjs::StringRef(JSON_LAYERS), layersJSON, alloc);

    rjs::Value weightsJSON(rjs::kArrayType);
    for(const auto &w : weights){
        for(int row = 0; row < w->rows(); row++){
            for(int col = 0; col < w->cols(); col++){
                weightsJSON.PushBack(rjs::Value(w->operator()(row, col)), alloc);
            }
        }
    }
    root.AddMember(rjs::StringRef(JSON_WEIGHTS), weightsJSON, alloc);

    rjs::Value biasJSON(rjs::kArrayType);
    for(auto b : bias){
        for(int col = 0; col < b->cols(); col++){
            biasJSON.PushBack(rjs::Value(b->operator()(0, col)), alloc);
        }
    }
    root.AddMember(rjs::StringRef(JSON_BIAS), biasJSON, alloc);
}

void NeuralNet::initFromJSON(rjs::Value &root) {
    auto layersJSON = root.FindMember(rjs::StringRef(JSON_LAYERS));
    auto weightsJSON = root.FindMember(rjs::StringRef(JSON_WEIGHTS));
    auto biasJSON = root.FindMember(rjs::StringRef(JSON_BIAS));

    if (layersJSON == root.MemberEnd() || !layersJSON->value.IsArray() ||
            weightsJSON == root.MemberEnd() || !weightsJSON->value.IsArray() ||
            biasJSON == root.MemberEnd() || !biasJSON->value.IsArray()) {
        Log().Get(logWARNING) << "\"" << JSON_LAYERS << "\", \"" << JSON_WEIGHTS << "\" or \"" << JSON_BIAS << "\""
                " not found or invalid in JSON, can't initialize";
        return;
    }

    layers.clear();
    for (const auto &layer : layersJSON->value.GetArray()) {
        layers.push_back(layer.GetInt());
    }

    auto weightsArray = weightsJSON->value.GetArray();

    weights.clear();
    int rows = layers.front();
    int idxJSON = 0;
    for(int i = 1; i < layers.size(); i++){
        int cols = layers[i];
        auto w = std::make_shared<Eigen::MatrixXf>();
        w->setZero(rows, cols);

        for(int j = 0; j < rows; j++){
            for(int k = 0; k < cols; k++){
                w->operator()(j, k) = weightsArray[idxJSON++].GetFloat();
            }
        }

        weights.push_back(w);
        rows = cols;
    }

    auto biasArray = biasJSON->value.GetArray();

    bias.clear();
    idxJSON = 0;
    for(int i = 1; i < layers.size(); i++){
        int cols = layers[i];

        auto b = std::make_shared<Eigen::MatrixXf>();
        b->setZero(1, cols);

        for(int j = 0; j < cols; j++){
            b->operator()(0, j) = biasArray[idxJSON++].GetFloat();
        }

        bias.push_back(b);
    }
}

std::shared_ptr<NeuralNet> NeuralNet::copy() {
    auto newCopy = std::make_shared<NeuralNet>(layers, activation);

    for(int i = 0; i < weights.size(); i++){
        newCopy->weights[i] = weights[i];
        newCopy->bias[i] = bias[i];
    }

    return newCopy;
}

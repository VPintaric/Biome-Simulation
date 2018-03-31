#include <memory>
#include <utility>
#include <state/State.h>

#include "minion/controllers/NeuralNetController.h"

NeuralNetController::NeuralNetController(const std::vector<int> &hiddenLayers = std::vector<int>(),
                                         std::function<float(float)> activation = tanhf) {

    std::vector<int> layers;
    layers.reserve(hiddenLayers.size() + 2);
    layers.push_back(INPUT_VARS);
    layers.insert(layers.end(), hiddenLayers.begin(), hiddenLayers.end());
    layers.push_back(OUTPUT_VARS);

    nn = std::make_shared<NeuralNet>(layers, activation);
};

NeuralNetController::~NeuralNetController() = default;

std::shared_ptr<NeuralNet> NeuralNetController::getNeuralNet() {
    return nn;
}

std::vector<float> NeuralNetController::controlMinion(std::vector<float> senseData) {
    auto rng = State::getInstance().getRng();
    std::normal_distribution<float> distr(0.f, 100.f);
    std::vector<float> v{distr(rng.get()), distr(rng.get())};
    return v;
}

//void NeuralNetController::controlMinion(const std::shared_ptr<MinionObject> &m,
//                                        const std::shared_ptr<MinionSenses> &senses) {
//    auto closest = senses->popSenseData();
//    auto sense = senses->popSenseData();
//    if(closest != nullptr){
//        while(sense != nullptr){
//            if(sense->dist < closest->dist){
//                closest = sense;
//            }
//            sense = senses->popSenseData();
//        }
//    }
//
//    Eigen::MatrixXf x(1, INPUT_VARS);
//    if(closest == nullptr){
//        x << senses->MAX_SENSE_DISTANCE, 0.f, 1.f;
//    } else {
//        x << closest->dist, closest->angle, 0.f;
//    }
//
//    x = nn->forward(x);
//
//    m->setControlForce(x(0, 0));
//    m->setControlRotMoment(x(0, 1));
//}

#include <memory>
#include <utility>

#include "minion/controllers/NeuralNetController.h"

NeuralNetController::NeuralNetController(const std::vector<int> &hiddenLayers = std::vector<int>(),
                                         std::function<float(float)> activation = tanhf)
                                        : activation(std::move(activation)) {
    int in = INPUT_VARS;
    for (int hiddenLayer : hiddenLayers) {
        auto w = std::make_shared<Eigen::MatrixXf>();
        w->setZero(in, hiddenLayer);
        auto b = std::make_shared<Eigen::MatrixXf>();
        b->setZero(1, hiddenLayer);
        weights.push_back(w);
        bias.push_back(b);
        in = hiddenLayer;
    }

    auto w = std::make_shared<Eigen::MatrixXf>();
    w->setZero(in, OUTPUT_VARS);
    auto b = std::make_shared<Eigen::MatrixXf>();
    b->setZero(1, OUTPUT_VARS);
    weights.push_back(w);
    bias.push_back(b);
};

NeuralNetController::~NeuralNetController() = default;

void NeuralNetController::controlMinion(const std::shared_ptr<MinionObject> &m,
                                        const std::shared_ptr<MinionSenses> &senses) {
    auto closest = senses->popSenseData();
    auto sense = senses->popSenseData();
    if(closest != nullptr){
        while(sense != nullptr){
            if(sense->dist < closest->dist){
                closest = sense;
            }
            sense = senses->popSenseData();
        }
    }

    Eigen::MatrixXf x(1, INPUT_VARS);
    if(closest == nullptr){
        x << senses->MAX_SENSE_DISTANCE, 0.f, 1.f;
    } else {
        x << closest->dist, closest->angle, 0.f;
    }

    for(int i = 0; i < weights.size(); i++){
        auto w = weights[i];
        auto b = bias[i];
        x *= *w;
        x += *b;
        x = x.unaryExpr(activation);
    }

    m->setControlForce(x(0, 0));
    m->setControlRotMoment(x(0, 1));
}

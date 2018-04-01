#include <minion/controllers/NeuralNetController.h>
#include <minion/senses/simple/SimpleMinionSenses.h>
#include <state/State.h>
#include "minion/factories/neuralnet/NeuralNetMinionGenerator.h"

NeuralNetMinionGenerator::NeuralNetMinionGenerator(std::vector<int> nnHiddenLayers)
                                                : nnHiddenLayers(nnHiddenLayers){

};

NeuralNetMinionGenerator::~NeuralNetMinionGenerator() = default;

std::shared_ptr<Minion> NeuralNetMinionGenerator::createRawMinion() {
    auto minion = std::make_shared<Minion>();
    auto object = std::make_shared<MinionObject>();
    auto senses = std::make_shared<SimpleMinionSenses>(minion);
    auto controller = std::shared_ptr<NeuralNetController>(new NeuralNetController(minion, nnHiddenLayers, tanhf));

    minion->setObject(object);
    minion->setSenses(senses);
    minion->setController(controller);

    return minion;
}

std::shared_ptr<Minion> NeuralNetMinionGenerator::generateMinion() {
    std::uniform_real_distribution<float> colorDistr(0.f, 1.f);
    std::uniform_real_distribution<float> radiusDistr(SimConst::MINION_MIN_RADIUS, SimConst::MINION_MAX_RADIUS);
    std::uniform_real_distribution<float> senseDistDistr(SimConst::MINION_MIN_MAX_SENSE_DISTANCE,
                                                           SimConst::MINION_MAX_MAX_SENSE_DISTANCE);

    auto minion = createRawMinion();
    auto object = minion->getObject();
    auto controller = std::static_pointer_cast<NeuralNetController>(minion->getController());
    auto senses = minion->getSenses();

    auto rng = State::getInstance().getRng();

    object->setSkinColor(glm::vec4(colorDistr(rng.get()), colorDistr(rng.get()), colorDistr(rng.get()), 1.f));
    object->setRadius(radiusDistr(rng.get()));

    controller->getNeuralNet()->initRandom();

    senses->setMaxSenseDistance(senseDistDistr(rng.get()));

    return minion;
}

std::shared_ptr<Minion> NeuralNetMinionGenerator::generateChild(std::shared_ptr<Minion> first,
                                                                std::shared_ptr<Minion> second) {
    auto child = crossover(first, second);
    mutate(child);
    return child;
}

std::shared_ptr<Minion> NeuralNetMinionGenerator::crossover(std::shared_ptr<Minion> first,
                                                            std::shared_ptr<Minion> second) {
    std::uniform_real_distribution<float> coeffDistr(0.f, 1.f);
    float coeff = coeffDistr(State::getInstance().getRng().get());

    auto child = createRawMinion();
    auto object = child->getObject();
    auto controller = std::static_pointer_cast<NeuralNetController>(child->getController());
    auto senses = child->getSenses();
    auto nn = controller->getNeuralNet();

    object->setSkinColor(coeff * first->getObject()->getSkinColor() + (1 - coeff) * second->getObject()->getSkinColor());
    object->setRadius(coeff * first->getObject()->getRadius() + (1 - coeff) * second->getObject()->getRadius());

    senses->setMaxSenseDistance(coeff * first->getSenses()->getMaxSenseDistance() +
                                (1 - coeff) * second->getSenses()->getMaxSenseDistance());

    auto firstNN = std::static_pointer_cast<NeuralNetController>(first->getController())->getNeuralNet();
    auto secondNN = std::static_pointer_cast<NeuralNetController>(second->getController())->getNeuralNet();
    for(int layer = 0; layer < nn->weights.size(); layer++){
        *nn->weights[layer] = coeff * *firstNN->weights[layer] + (1 - coeff) * *secondNN->weights[layer];
        *nn->bias[layer] = coeff * *firstNN->bias[layer] + (1 - coeff) * *secondNN->bias[layer];
    }

    return child;
}

void NeuralNetMinionGenerator::mutate(std::shared_ptr<Minion> m) {
    std::normal_distribution<float> colorMut(0.f, 1.f);
    std::normal_distribution<float> radiusMut(0.f, 5.f);
    std::normal_distribution<float> senseDistMut(0.f, 5.f);
    std::normal_distribution<float> nnMut(0.f, 10.f);

    auto rng = State::getInstance().getRng();

    auto object = m->getObject();
    auto controller = std::static_pointer_cast<NeuralNetController>(m->getController());
    auto senses = m->getSenses();
    auto nn = controller->getNeuralNet();

    auto c = object->getSkinColor();
    object->setSkinColor(glm::vec4(c.r + colorMut(rng.get()), c.g + colorMut(rng.get()), c.b + colorMut(rng.get()), 1.f));
    object->setRadius(object->getRadius() + radiusMut(rng.get()));
    senses->setMaxSenseDistance(senses->getMaxSenseDistance() + senseDistMut(rng.get()));

    for(int layer = 0; layer < nn->weights.size(); layer++){
        auto w = nn->weights[layer];
        auto b = nn->bias[layer];
        for(int c = 0; c < w->cols(); c++){
            b->operator()(0, c) += nnMut(rng.get());
            for(int r = 0; r < w->rows(); r++){
                w->operator()(r, c) += nnMut(rng.get());
            }
        }
    }
}
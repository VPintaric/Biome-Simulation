#include "minion/controllers/NeuralNetController.h"
#include "eigen/Dense"

NeuralNetController::NeuralNetController() = default;
NeuralNetController::~NeuralNetController() = default;

void NeuralNetController::controlMinion(const std::shared_ptr<MinionObject> &m,
                                        const std::shared_ptr<MinionSenses> &senses) {
    float rotMom, force;
    rotMom = force = 0.f;

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

    if(closest != nullptr){

    }

    m->setControlForce(force);
    m->setControlRotMoment(rotMom);
}

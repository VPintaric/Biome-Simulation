#ifndef BIOME_SIMULATION_MINIONGENERATOR_H
#define BIOME_SIMULATION_MINIONGENERATOR_H


#include <random>
#include "../Minion.h"
#include "../../constants/WindowConstants.h"

class MinionGenerator {
public:
    ~MinionGenerator();
    static MinionGenerator& getInstance();

    std::shared_ptr<Minion> generateMinion();
private:

    std::uniform_real_distribution<float> posDistr, angleDistr, typeDistr;
    std::normal_distribution<float> radiusDistr;

    const int NUM_TYPES = 4;

    const std::vector<float> TYPE_PROBS = {
            .2f, .4f, 0.3f, 0.1f
    };

    const std::vector<glm::vec4> COLOR_TYPES = {
        glm::vec4(0.5f, 0.5f, 0.5f, 1.f),
        glm::vec4(0.f, 1.f, 0.f, 1.f),
        glm::vec4(1.f, 0.f, 0.f, 1.f),
        glm::vec4(0.2f, 0.2f, 0.6f, 1.f),
    };

    std::shared_ptr<MinionController> getControllerForType(int type);
    int getRandomType();

    MinionGenerator();
};


#endif //BIOME_SIMULATION_MINIONGENERATOR_H

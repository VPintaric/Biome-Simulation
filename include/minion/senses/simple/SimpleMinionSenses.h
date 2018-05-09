#ifndef BIOME_SIMULATION_SIMPLEMINIONSENSES_H
#define BIOME_SIMULATION_SIMPLEMINIONSENSES_H

#include <minion/senses/MinionSenses.h>
#include <rendering/ColorModel.h>
#include <constants/SimulationConstants.h>
#include <map>

class SimpleMinionSenses : public MinionSenses {
public:
    explicit SimpleMinionSenses(std::weak_ptr<Minion> minion,
                                float maxSenseDistance = SimConst::MINION_DEFAULT_MAX_SENSE_DISTANCE,
                                int nSightLines = SimConst::MINION_DEFAULT_N_SIGHT_LINES);
    ~SimpleMinionSenses() override;

    void setMaxSenseDistance(float dist) override;

    float getMaxSenseDistance() override;

    void setNSightLines(int n);

    void setSightLineColor(int idx, glm::vec3 color);

    int getDataSize() override;

    std::vector<float> gatherData(float deltaT) override;

    void standardizeData(std::vector<float> &data) override;

    void draw() override;

    std::shared_ptr<MinionSenses> copy() override;

    void setMinion(std::shared_ptr<Minion> m) override;

private:
    void createNewSightLinesModel();
    std::shared_ptr<Minion> lockMinion();

    const float COLOR_ALPHA = 0.04f;
    const int N_SIGHT_LINE_CHECK_POINTS = 50;

    enum OBJECT_TYPE {
        NOTHING_TYPE = 0,
        DEAD_MINION_TYPE = 10,
        ALIVE_MINION_TYPE = 20,
        FOOD_PELLET_TYPE = 30,
        POISON_PELLET_TYPE = 40,
        BOUNDARY_TYPE = 50
    };

    std::map<int, glm::vec3> OBJECT_TYPE_TO_COLOR = {
            {NOTHING_TYPE, glm::vec3(1.f, 1.f, 1.f)},
            {DEAD_MINION_TYPE, glm::vec3(0.f, 1.f, 0.5f)},
            {ALIVE_MINION_TYPE, glm::vec3(1.f, 1.f, 0.f)},
            {FOOD_PELLET_TYPE, glm::vec3(0.f, 1.f, 0.f)},
            {POISON_PELLET_TYPE, glm::vec3(0.73f, 0.33f, 0.83f)},
            {BOUNDARY_TYPE, glm::vec3(1.f, 0.f, 0.f)},
    };

    float maxSenseDistance;
    int nSightLines;

    std::unique_ptr<ColorModel> sightLinesModel;
    std::weak_ptr<Minion> minion;
};

#endif //BIOME_SIMULATION_SIMPLEMINIONSENSES_H

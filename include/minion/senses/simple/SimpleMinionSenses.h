#ifndef BIOME_SIMULATION_SIMPLEMINIONSENSES_H
#define BIOME_SIMULATION_SIMPLEMINIONSENSES_H

#include <minion/senses/MinionSenses.h>
#include <rendering/ColorModel.h>
#include <constants/SimulationConstants.h>

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

    void draw() override;

    std::shared_ptr<MinionSenses> copy() override;

    void setMinion(std::shared_ptr<Minion> m) override;

private:
    void createNewSightLinesModel();
    std::shared_ptr<Minion> lockMinion();

    const float COLOR_ALPHA = 0.04f;
    const int N_SIGHT_LINE_CHECK_POINTS = 50;

    const float NOTHING_TYPE = 0.f;
    const float DEAD_MINION_TYPE = 2.5f;
    const float ALIVE_MINION_TYPE = 5.f;
    const float PELLET_TYPE = 7.5f;
    const float BOUNDARY_TYPE = 10.f;

    float maxSenseDistance;
    int nSightLines;

    std::unique_ptr<ColorModel> sightLinesModel;
    std::weak_ptr<Minion> minion;
};

#endif //BIOME_SIMULATION_SIMPLEMINIONSENSES_H

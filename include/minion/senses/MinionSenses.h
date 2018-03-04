#ifndef BIOME_SIMULATION_MINIONSENSES_H
#define BIOME_SIMULATION_MINIONSENSES_H


#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include <minion/object/MinionObject.h>

class MinionSenses : public CircleObject {
public:
    class SenseData{
    public:
        SenseData() = default;
        ~SenseData() = default;

        float angle{};
        float dist{};
        glm::vec4 color;
    };

    const float MAX_SENSE_DISTANCE = 70.f;

    MinionSenses();
    virtual ~MinionSenses();

    virtual void gatherData(std::shared_ptr<MinionObject> minion);
    virtual std::shared_ptr<SenseData> popSenseData();
    virtual void clearData();
    virtual void draw();

private:
    std::vector< std::shared_ptr<SenseData> > senseDatas;
};


#endif //BIOME_SIMULATION_MINIONSENSES_H

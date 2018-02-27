#ifndef BIOME_SIMULATION_MINIONSENSES_H
#define BIOME_SIMULATION_MINIONSENSES_H


#include <GL/glm/glm.hpp>
#include <vector>
#include <memory>

class MinionSenses {
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

    virtual void addSenseData(const std::shared_ptr<SenseData>& si);
    virtual std::shared_ptr<SenseData> popSenseData();
    virtual void clearData();

private:
    std::vector< std::shared_ptr<SenseData> > senseDatas;
};


#endif //BIOME_SIMULATION_MINIONSENSES_H

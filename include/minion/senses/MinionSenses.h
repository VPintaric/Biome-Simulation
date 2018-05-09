#ifndef BIOME_SIMULATION_MINIONSENSES_H
#define BIOME_SIMULATION_MINIONSENSES_H

#include <objects/CircleObject.h>
#include <vector>
#include <minion/Minion.h>

class Minion;

class MinionSenses : public CircleObject, public JSONPersistable, public Copyable<MinionSenses> {
public:
    MinionSenses();

    virtual ~MinionSenses();

    virtual void setMaxSenseDistance(float dist) = 0;

    virtual float getMaxSenseDistance() = 0;

    virtual int getDataSize() = 0;

    virtual std::vector<float> gatherData(float deltaT) = 0;

    virtual void standardizeData(std::vector<float> &data) = 0;

    virtual void draw() = 0;

    void persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) override;

    void initFromJSON(rjs::Value &root) override;

    virtual void setMinion(std::shared_ptr<Minion> m) = 0;

protected:
    const char * JSON_MAX_SENSE_DISTANCE = "max_sense_distance";
};


#endif //BIOME_SIMULATION_MINIONSENSES_H

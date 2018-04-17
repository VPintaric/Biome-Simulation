#ifndef BIOME_SIMULATION_MINIONCONTROLLER_H
#define BIOME_SIMULATION_MINIONCONTROLLER_H

#include "minion/object/MinionObject.h"
#include "minion/senses/MinionSenses.h"

class MinionController : public JSONPersistable, public Copyable<MinionController> {
public:
    MinionController();
    virtual ~MinionController();

    virtual std::vector<float> controlMinion(std::vector<float> senseData) = 0;

    void persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) override;

    void initFromJSON(rjs::Value &root) override;
};


#endif //BIOME_SIMULATION_MINIONCONTROLLER_H

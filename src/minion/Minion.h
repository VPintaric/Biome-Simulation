#ifndef BIOME_SIMULATION_MINION_H
#define BIOME_SIMULATION_MINION_H

#include <memory>
#include "object/MinionObject.h"
#include "senses/MinionSenses.h"
#include "controllers/MinionController.h"

class Minion {
public:
    Minion();
    virtual ~Minion();

    const std::shared_ptr<MinionObject> &getObject() const;

    void setObject(const std::shared_ptr<MinionObject> &object);

    const std::shared_ptr<MinionSenses> &getSenses() const;

    void setSenses(const std::shared_ptr<MinionSenses> &senses);

    const std::shared_ptr<MinionController> &getController() const;

    void setController(const std::shared_ptr<MinionController> &controller);

    void update(float deltaT);
    void draw();

private:

    std::shared_ptr<MinionObject> object;
    std::shared_ptr<MinionSenses> senses;
    std::shared_ptr<MinionController> controller;
};


#endif //BIOME_SIMULATION_MINION_H

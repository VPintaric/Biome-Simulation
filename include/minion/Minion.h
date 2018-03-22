#ifndef BIOME_SIMULATION_MINION_H
#define BIOME_SIMULATION_MINION_H

#include <memory>
#include "minion/object/MinionObject.h"
#include "minion/senses/MinionSenses.h"
#include "minion/controllers/MinionController.h"

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
    void control(float deltaT);
    void draw();

    float getMaxLife() const;

    void setMaxLife(float maxLife);

    float getMinLife() const;

    void setMinLife(float minLife);

    float getLife() const;

    void setLife(float life);

    float getDecay() const;

    void setDecay(float decay);

    float getTimeLived() const;

    void setTimeLived(float timeLived);

    bool isDead() const;

    void setDead(bool dead);

    bool isDecayed() const;

    void setDecayed(bool decayed);

    int getId() const;

    void setId(int id);

private:

    std::shared_ptr<MinionObject> object;
    std::shared_ptr<MinionSenses> senses;
    std::shared_ptr<MinionController> controller;

    int id;
    float maxLife, minLife, life, decay, timeLived;
    bool dead, decayed;
};


#endif //BIOME_SIMULATION_MINION_H

#ifndef BIOME_SIMULATION_MINION_H
#define BIOME_SIMULATION_MINION_H

#include <memory>
#include "minion/object/MinionObject.h"
#include "minion/senses/MinionSenses.h"
#include "minion/controllers/MinionController.h"
#include <rapidjson/document.h>
#include <string>
#include <persistence/JSONPersistable.h>
#include <copyable/Copyable.h>

namespace rjs = rapidjson;

class MinionSenses;
class MinionController;

class Minion : public JSONPersistable, public Copyable<Minion> {
public:
    Minion();
    virtual ~Minion();

    std::shared_ptr<Minion> copy() override;

    const std::shared_ptr<MinionObject> &getObject() const;

    void setObject(const std::shared_ptr<MinionObject> &object);

    const std::shared_ptr<MinionSenses> &getSenses() const;

    void setSenses(const std::shared_ptr<MinionSenses> &senses);

    const std::shared_ptr<MinionController> &getController() const;

    void setController(const std::shared_ptr<MinionController> &controller);

    void persistToJSON(rjs::Value& root, rjs::Document::AllocatorType& alloc) override;

    void initFromJSON(rjs::Value &root) override;

    void update(float deltaT);

    void control(float deltaT);

    void draw(bool drawSenses = true);

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

    float getDistanceTraveled() const;

    void setDistanceTraveled(float distanceTraveled);

    float getDamageDealt() const;

    void setDamageDealt(float damageDealt);

    float getHealthRecovered() const;

    void setHealthRecovered(float healthRecovered);

    float getFitness() const;

    void setFitness(float fitness);

private:
    std::shared_ptr<MinionObject> object;
    std::shared_ptr<MinionSenses> senses;

    std::shared_ptr<MinionController> controller;
    int id;
    float maxLife, minLife, life, decay;

    bool dead, decayed;
    float timeLived, distanceTraveled, damageDealt, healthRecovered;
    float fitness;

    const char * JSON_OBJECT = "object";
    const char * JSON_SENSES = "senses";
    const char * JSON_CONTROLLER = "controller";
};


#endif //BIOME_SIMULATION_MINION_H

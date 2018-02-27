#ifndef MINION_OBJECT_H
#define MINION_OBJECT_H

#include <GL/glm/glm.hpp>
#include <memory>
#include "../../rendering/Model.h"
#include "../../objects/CircleObject.h"


class MinionObject : public CircleObject{
public:
    MinionObject();
    virtual ~MinionObject();
    
    void setSkinColor(glm::vec4 c);
    void setMaxLife(float l);
    void setLife(float l);
    void setDecay(float d);
    
    glm::vec4 getSkinColor() const;
    glm::vec4 getBodyColor() const;
    float getMaxLife() const;
    float getLife() const;
    float getDecay() const;

    void draw() const;
    void update(float deltaT) override;

    bool isDead() const;

    void setDead(bool dead);

    float getControlForce() const;

    void setControlForce(float controlForce);

    float getControlRotMoment() const;

    void setControlRotMoment(float controlRotMoment);

    float getMinLife() const;

    void setMinLife(float minLife);

    bool isDecayed() const;

    void setDecayed(bool decayed);

private:
    std::shared_ptr<Model> bodyModel, circleModel;

    glm::vec4 skinColor;
    float maxLife, minLife, life, decay, controlForce, controlRotMoment;

    bool dead, decayed;
};

#endif /* MINION_OBJECT_H */


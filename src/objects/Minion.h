#ifndef MINION_H
#define MINION_H

#include <GL/glm/glm.hpp>
#include <memory>
#include "../rendering/Model.h"
#include "CircleObject.h"


class Minion : public CircleObject{
public:
    Minion();
    virtual ~Minion();
    
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

    bool isIsDead() const;

    void setIsDead(bool isDead);

private:
    std::shared_ptr<Model> bodyModel, circleModel;

    glm::vec4 skinColor;
    float maxLife, life, decay;

    bool isDead;
};

#endif /* MINION_H */


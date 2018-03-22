#ifndef MINION_OBJECT_H
#define MINION_OBJECT_H

#include "glm/glm.hpp"
#include <memory>
#include "rendering/Model.h"
#include "objects/CircleObject.h"


class MinionObject : public CircleObject{
public:
    MinionObject();
    virtual ~MinionObject();
    
    void setSkinColor(glm::vec4 c);

    glm::vec4 getSkinColor() const;

    glm::vec4 getBodyColor() const;

    void setBodyColor(glm::vec4 color);

    void draw() const;

    void update(float deltaT) override;

    float getControlForce() const;

    void setControlForce(float controlForce);

    float getControlRotMoment() const;

    void setControlRotMoment(float controlRotMoment);

private:
    std::shared_ptr<Model> bodyModel, circleModel;

    glm::vec4 skinColor, bodyColor;
    float controlForce, controlRotMoment;
};

#endif /* MINION_OBJECT_H */


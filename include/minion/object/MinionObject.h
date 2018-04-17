#ifndef MINION_OBJECT_H
#define MINION_OBJECT_H

#include "glm/glm.hpp"
#include <memory>
#include "rendering/Model.h"
#include "objects/CircleObject.h"
#include <persistence/JSONPersistable.h>
#include <copyable/Copyable.h>

class MinionObject : public CircleObject, public JSONPersistable, public Copyable<MinionObject> {
public:
    MinionObject();
    virtual ~MinionObject();

    void persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) override;

    void initFromJSON(rjs::Value &root) override;

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

    std::shared_ptr<MinionObject> copy() override;

private:
    std::shared_ptr<Model> bodyModel, circleModel;

    glm::vec4 skinColor, bodyColor;
    float controlForce, controlRotMoment;

    const char * JSON_SKIN_COLOR = "skin_color";
    const char * JSON_RADIUS = "radius";
};

#endif /* MINION_OBJECT_H */


#ifndef MINION_H
#define MINION_H

#include <GL/glm/glm.hpp>
#include <memory>
#include "../rendering/Model.h"
#include "CircleObject.h"


class Minion : public CircleObject{
public:
    Minion(std::shared_ptr<Model> m);
    virtual ~Minion();
    
    void setColor(glm::vec4 c);
    void setModel(std::shared_ptr<Model> m);
    void setMaxLife(float l);
    void setLife(float l);
    void setDecay(float d);
    
    glm::vec4 getColor() const;
    std::shared_ptr<Model> getModel() const;
    float getMaxLife() const;
    float getLife() const;
    float getDecay() const;

    void draw() const;
    void update(float deltaT);
    
private:
    std::shared_ptr<Model> model;

    glm::vec4 color;
    float maxLife, life, decay;
};

#endif /* MINION_H */


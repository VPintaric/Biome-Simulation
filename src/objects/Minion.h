#ifndef MINION_H
#define MINION_H

#include <GL/glm/glm.hpp>
#include <memory>
#include "../rendering/Model.h"


class Minion {
public:
    Minion(std::shared_ptr<Model> m,
            glm::vec2 p = glm::vec2(0., 0.), 
            float angle = 0.,
            glm::vec2 v = glm::vec2(0., 0.),
            glm::vec4 c = glm::vec4(1., 1., 1., 1.),
            float s = 1., float mass = 1.);
    virtual ~Minion();
    
    void setPos(glm::vec2 p);
    void setAngle(float a);
    void setVelocity(glm::vec2 v);
    void setColor(glm::vec4 c);
    void setScale(float s);
    void setMass(float m);
    void setModel(std::shared_ptr<Model> m);
    void setMaxLife(float l);
    void setLife(float l);
    void setDecay(float d);
    
    glm::vec2 getPos() const;
    float getAngle() const;
    glm::vec2 getVelocity() const;
    glm::vec4 getColor() const;
    float getScale() const;
    float getMass() const;
    std::shared_ptr<Model> getModel() const;
    float getMaxLife() const;
    float getLife() const;
    float getDecay() const;
    
    void draw() const;
    void update(float deltaT);
    
private:
    std::shared_ptr<Model> model;
    
    glm::vec2 pos, velocity;
    float angle;
    glm::vec4 color;
    float scale, mass;
    
    float maxLife, life, decay;
};

#endif /* MINION_H */


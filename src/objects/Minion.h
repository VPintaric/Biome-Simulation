#ifndef MINION_H
#define MINION_H

#include <GL/glm/glm.hpp>
#include <memory>
#include "../rendering/Model.h"


class Minion {
public:
    Minion(std::shared_ptr<Model> m,
            glm::vec2 p = glm::vec2(0.f, 0.f), 
            float angle = 0.f,
            glm::vec2 v = glm::vec2(0.f, 0.f),
            glm::vec4 c = glm::vec4(1.f, 1.f, 1.f, 1.f),
            float s = 1.f, float mass = 1.f);
    virtual ~Minion();
    
    void setPos(glm::vec2 p);
    void setAngle(float a);
    void setVelocity(glm::vec2 v);
    void setColor(glm::vec4 c);
    void setScale(float s);
    void setMass(float m);
    void setModel(std::shared_ptr<Model> m);
    
    glm::vec2 getPos() const;
    float getAngle() const;
    glm::vec2 getVelocity() const;
    glm::vec4 getColor() const;
    float getScale() const;
    float getMass() const;
    std::shared_ptr<Model> getModel() const;
    
    void draw() const;
    void update(float deltaT);
    
private:
    std::shared_ptr<Model> model;
    
    glm::vec2 pos, velocity;
    float angle;
    glm::vec4 color;
    float scale, mass;
};

#endif /* MINION_H */


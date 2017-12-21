#ifndef MINION_H
#define MINION_H

#include <GL/glm/glm.hpp>
#include <memory>
#include "../rendering/Model.h"
#include "../collision/CollisionCircle.h"


class Minion {
public:
    Minion(std::shared_ptr<Model> m,
            glm::vec2 p = glm::vec2(0.f, 0.f), 
            float angle = 0.f,
            float angleAcc = 0.f,
            glm::vec2 v = glm::vec2(0.f, 0.f),
            float acc = 0.f,
            glm::vec4 c = glm::vec4(1.f, 1.f, 1.f, 1.f),
            float s = 1.f, float mass = 1.f);
    virtual ~Minion();
    
    void setPos(glm::vec2 p);
    void setAngle(float a);
    void setAngleAcc(float aAcc);
    void setVelocity(glm::vec2 v);
    void setAcceleration(float a);
    void setColor(glm::vec4 c);
    void setScale(float s);
    void setMass(float m);
    void setModel(std::shared_ptr<Model> m);
    void setMaxLife(float l);
    void setLife(float l);
    void setDecay(float d);
    
    glm::vec2 getPos() const;
    float getAngle() const;
    float getAngleAcc() const;
    glm::vec2 getVelocity() const;
    float getAcceleration() const;
    glm::vec4 getColor() const;
    float getScale() const;
    float getMass() const;
    std::shared_ptr<Model> getModel() const;
    float getMaxLife() const;
    float getLife() const;
    float getDecay() const;
    std::shared_ptr<CollisionCircle> getCollBounds() const;
    
    void draw() const;
    void update(float deltaT);
    
private:
    std::shared_ptr<Model> model;
    std::shared_ptr<CollisionCircle> collisionBounds;
    
    glm::vec2 pos, velocity;
    float angle, angleAcc, acceleration;
    glm::vec4 color;
    float scale, mass;
    
    float maxLife, life, decay;
};

#endif /* MINION_H */


#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include "Minion.h"
#include "../rendering/Renderer.h"
#include "../constants/SimulationConstants.h"
#include "../state/Log.h"
#include "../state/State.h"

#include <GL/glm/gtc/quaternion.hpp>
#include <GL/glm/gtx/quaternion.hpp>
#include <GL/glm/gtx/vector_angle.hpp>
#include <GL/glm/gtc/type_ptr.hpp>
#include <random>

Minion::Minion() {
    Renderer &r = Renderer::getInstance();
    bodyModel = r.getModel(SimConst::MINION_BODY_MODEL_NAME);
    circleModel = r.getModel(SimConst::MINION_CIRCLE_MODEL_NAME);

    isDead = false;
}

Minion::~Minion() {
}

void Minion::draw() const {
    glm::vec2 pos = getPos();
    float angle = getAngle();
    float scale = getRadius();

    Renderer &r = Renderer::getInstance();
    
    GLuint shaderId = r.getActiveShaderProgram()->getId();

    r.pushMatrix(Renderer::Matrix::MODEL);
    r.transform(pos.x, pos.y, 0.f,
            angle, scale, scale);

    glUniform4fv(glGetUniformLocation(shaderId, "color_mod"), 1, glm::value_ptr(skinColor));
    circleModel->draw();

    glUniform4fv(glGetUniformLocation(shaderId, "color_mod"), 1, glm::value_ptr(getBodyColor()));
    bodyModel->draw();

    r.popMatrix(Renderer::Matrix::MODEL);
}

void Minion::update(float deltaT) {
    const float FRICTION_COEFF_TRANSL = 1e-3f;
    const float FRICTION_COEFF_ROT = 0.5f;

    auto rng = State::getInstance().getRng();
    std::normal_distribution<float> momDistr(0.f, 5000.f);
    std::normal_distribution<float> forceDistr(200.f, 5.f);

    float cAngleAcc = isDead ? 0.f : rInertia * momDistr(rng.get());;
    setAngleVel(angleVel + deltaT * (angleAcc + cAngleAcc - FRICTION_COEFF_ROT * std::fabs(angleVel) * angleVel));
    setAngle(angle + deltaT * angleVel);

    float cAcc = isDead ? 0.f : rMass * forceDistr(rng.get());
    glm::vec2 orient = glm::rotate(glm::vec2(0.f, 1.f), angle);
    setVelocity(velocity + deltaT * (acceleration + cAcc * orient -
                        FRICTION_COEFF_TRANSL * getRadius() * glm::length2(velocity) * velocity));
    setPos(pos + deltaT * velocity);

    life -= deltaT * decay;
    if(life <= 0.f){
        setIsDead(true);
    }
}

glm::vec4 Minion::getBodyColor() const {
    return isDead ? glm::vec4(0.2f, 0.f, 0.f, 1.f) : glm::vec4(1.f - life / maxLife, life / maxLife, 0.f, 1.f);
}

glm::vec4 Minion::getSkinColor() const {
    return skinColor;
}

float Minion::getMaxLife() const {
    return maxLife;
}

float Minion::getLife() const {
    return life;
}

float Minion::getDecay() const {
    return decay;
}

void Minion::setSkinColor(glm::vec4 c) {
    skinColor = c;
}

void Minion::setMaxLife(float l) {
    maxLife = l;
}

void Minion::setLife(float l) {
    life = l;
}

void Minion::setDecay(float d) {
    decay = d;
}

bool Minion::isIsDead() const {
    return isDead;
}

void Minion::setIsDead(bool isDead) {
    Minion::isDead = isDead;
}

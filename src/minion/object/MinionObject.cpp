#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include "minion/object/MinionObject.h"
#include "rendering/Renderer.h"
#include "constants/SimulationConstants.h"
#include "helpers/MathHelpers.h"

#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtc/type_ptr.hpp"

MinionObject::MinionObject() {
    Renderer &r = Renderer::getInstance();
    bodyModel = r.getModel(SimConst::MINION_BODY_MODEL_NAME);
    circleModel = r.getModel(SimConst::MINION_CIRCLE_MODEL_NAME);

    dead = false;
    decayed = false;
}

MinionObject::~MinionObject() = default;

void MinionObject::draw() const {
    glm::vec2 pos = getPos();
    float angle = getAngle();
    float scale = getRadius();

    Renderer &r = Renderer::getInstance();
    
    GLuint shaderId = r.getActiveShaderProgram()->getId();

    r.pushMatrix(Renderer::Matrix::MODEL);
    r.transform(pos.x, pos.y, -1.f,
            angle, scale, scale);

    glUniform4fv(glGetUniformLocation(shaderId, "color_mod"), 1, glm::value_ptr(skinColor));
    circleModel->draw();

    glUniform4fv(glGetUniformLocation(shaderId, "color_mod"), 1, glm::value_ptr(getBodyColor()));
    bodyModel->draw();

    r.popMatrix(Renderer::Matrix::MODEL);
}

void MinionObject::update(float deltaT) {
    float rotAcc = isDead() ? 0.f : rInertia * controlRotMoment;
    glm::vec2 acc = isDead() ? glm::vec2(0.f, 0.f) : rMass * controlForce * front;

    setAcceleration(getAcceleration() + acc);
    setAngleAcc(getAngleAcc() + rotAcc);

    CircleObject::update(deltaT);

    life -= deltaT * decay;

    if(life <= 0.f){
        setDead(true);
    }

    if(life <= minLife){
        setDecayed(true);
    }
}

glm::vec4 MinionObject::getBodyColor() const {
    return dead ? glm::vec4(1.f, 1.f, 1.f, 1.f - (life / minLife))
                : glm::vec4(1.f - life / maxLife, life / maxLife, 0.f, 1.f);
}

glm::vec4 MinionObject::getSkinColor() const {
    return skinColor;
}

float MinionObject::getMaxLife() const {
    return maxLife;
}

float MinionObject::getLife() const {
    return life;
}

float MinionObject::getDecay() const {
    return decay;
}

void MinionObject::setSkinColor(glm::vec4 c) {
    skinColor = c;
}

void MinionObject::setMaxLife(float l) {
    maxLife = l;
}

void MinionObject::setLife(float l) {
    life = l;

    if(life > 0.f){
        dead = decayed = false;
        if(life > maxLife){
            life = maxLife;
        }
    } else if(life <= 0.f && life > minLife){
        dead = true;
        decayed = false;
    } else {
        dead = decayed = true;
    }
}

void MinionObject::setDecay(float d) {
    decay = d;
}

bool MinionObject::isDead() const {
    return dead;
}

void MinionObject::setDead(bool dead) {
    MinionObject::dead = dead;
}

float MinionObject::getMinLife() const {
    return minLife;
}

void MinionObject::setMinLife(float minLife) {
    MinionObject::minLife = minLife;
}

bool MinionObject::isDecayed() const {
    return decayed;
}

void MinionObject::setDecayed(bool decayed) {
    MinionObject::decayed = decayed;
}

float MinionObject::getControlForce() const {
    return controlForce;
}

void MinionObject::setControlForce(float controlForce) {
    const float MAX_CONTROL_FORCE = 500.f;
    const float MIN_CONTROL_FORCE = -0.3f * MAX_CONTROL_FORCE;

    MinionObject::controlForce = Math::clamp(controlForce, MIN_CONTROL_FORCE, MAX_CONTROL_FORCE);
}

float MinionObject::getControlRotMoment() const {
    return controlRotMoment;
}

void MinionObject::setControlRotMoment(float controlRotMoment) {
    const float MAX_CONTROL_ROT_MOMENT = 10000.f;
    MinionObject::controlRotMoment = Math::clamp(controlRotMoment, -MAX_CONTROL_ROT_MOMENT, MAX_CONTROL_ROT_MOMENT);
}

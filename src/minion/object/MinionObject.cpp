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
    float rotAcc = rInertia * controlRotMoment;
    glm::vec2 acc = rMass * controlForce * front;

    setAcceleration(getAcceleration() + acc);
    setAngleAcc(getAngleAcc() + rotAcc);

    CircleObject::update(deltaT);
}

void MinionObject::setBodyColor(glm::vec4 color){
    bodyColor = color;
}

glm::vec4 MinionObject::getBodyColor() const {
    return bodyColor;
}

glm::vec4 MinionObject::getSkinColor() const {
    return skinColor;
}

void MinionObject::setSkinColor(glm::vec4 c) {
    skinColor = c;
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
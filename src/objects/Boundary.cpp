#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include "glm/gtc/type_ptr.hpp"
#include "Boundary.h"
#include "../rendering/Renderer.h"
#include "../constants/SimulationConstants.h"


Boundary::Boundary(){
    model = Renderer::getInstance().getModel(SimConst::HOLLOW_CIRCLE_MODEL_NAME);
    HollowCircleObject::setR2(1.f);
    HollowCircleObject::setRMass(0.f);
}

Boundary::~Boundary() = default;

void Boundary::draw() {
    glm::vec2 pos = getPos();
    float angle = getAngle();
    float scale = getR2();

    Renderer &r = Renderer::getInstance();

    GLuint shaderId = r.getActiveShaderProgram()->getId();
    glUniform4fv(glGetUniformLocation(shaderId, "color_mod"), 1, glm::value_ptr(color));

    r.pushMatrix(Renderer::Matrix::MODEL);
    r.transform(pos.x, pos.y, 0.f,
                angle, scale, scale);
    model->draw();
    r.popMatrix(Renderer::Matrix::MODEL);
}

const glm::vec4 &Boundary::getColor() const {
    return color;
}

void Boundary::setColor(const glm::vec4 &color) {
    Boundary::color = color;
}
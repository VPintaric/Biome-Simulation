#include <GL/glew.h>
#include <constants/SimulationConstants.h>
#include <rendering/Renderer.h>
#include <glm/gtc/type_ptr.hpp>
#include "objects/Pellet.h"

Pellet::Pellet(bool isFood) : isFoodAttr(isFood) {
    setRadius(SimConst::PELLET_RADIUS);
    model = Renderer::getInstance().getModel(SimConst::FULL_CIRCLE_MODEL_NAME);
}

void Pellet::draw() {
    if(life > 0.f) {
        glm::vec2 pos = getPos();
        float angle = getAngle();
        float scale = getRadius();

        Renderer &r = Renderer::getInstance();

        GLuint shaderId = r.getActiveShaderProgram()->getId();
        glUniform4fv(glGetUniformLocation(shaderId, "color_mod"), 1, glm::value_ptr(color));

        r.pushMatrix(Renderer::Matrix::MODEL);
        r.transform(pos.x, pos.y, 0.f,
                    angle, scale, scale);
        model->draw();
        r.popMatrix(Renderer::Matrix::MODEL);
    }
}

const glm::vec4& Pellet::getColor() const {
    return color;
}

void Pellet::setColor(const glm::vec4 &color) {
    Pellet::color = color;
}

float Pellet::getLife() const {
    return life;
}

void Pellet::setLife(float life) {
    Pellet::life = life;
}

bool Pellet::isFood() const {
    return isFoodAttr;
}

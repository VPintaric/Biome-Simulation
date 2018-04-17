#include "objects/Pellet.h"

void Pellet::draw() {

}

const glm::vec4& Pellet::getColor() const {
    return color;
}

void Pellet::setColor(const glm::vec4 &color) {
    Pellet::color = color;
}
#include "Camera.h"
#include "Renderer.h"

#include "glm/gtx/vector_query.hpp"

Camera& Camera::getInstance() {
    static Camera instance;
    return instance;
}

Camera::Camera() : pos(0.f, 0.f, 0.f), moveDirection(0.f, 0.f, 0.f), speed(1.f){

}

Camera::~Camera() {

}

const glm::vec3 &Camera::getPos() const {
    return pos;
}

void Camera::setPos(const glm::vec3 &pos) {
    Renderer &r = Renderer::getInstance();

    Camera::pos = pos;

    r.setCameraPosition(pos.x, pos.y, pos.z);
}

float Camera::getSpeed() const {
    return speed;
}

void Camera::setSpeed(float speed) {
    Camera::speed = speed;
}

const glm::vec3 &Camera::getMoveDirection() const {
    return moveDirection;
}

void Camera::update(float deltaT) {
    // scale the speed of camera movement depending on the current zoom
    // i.e. slow the camera down when you are zoomed in and speed it up when zoomed out so the user
    // can move it with more precision
    const float LINEAR_SCALE_ZOOM_FACTOR = 1e-3f;
    setPos(pos + LINEAR_SCALE_ZOOM_FACTOR * std::fabs(pos.z) * speed * moveDirection);
}

void Camera::startMoveInDirection(Direction dir) {
    switch(dir){
        case UP:
            moveDirection.y = 1.f;
            break;
        case DOWN:
            moveDirection.y = -1.f;
            break;
        case LEFT:
            moveDirection.x = 1.f;
            break;
        case RIGHT:
            moveDirection.x = -1.f;
            break;
        case IN:
            moveDirection.z = 1.f;
            break;
        case OUT:
            moveDirection.z = -1.f;
            break;
        default:
            break;
    }
    moveDirection = glm::normalize(moveDirection);
}

void Camera::stopMoveInDirection(Direction dir) {
    switch(dir){
        case UP:
        case DOWN:
            moveDirection.y = 0.f;
            break;
        case LEFT:
        case RIGHT:
            moveDirection.x = 0.f;
            break;
        case OUT:
        case IN:
            moveDirection.z = 0.f;
            break;
        default:
            break;
    }

    if(!glm::isNull(moveDirection, 1e-6f)){
        moveDirection = glm::normalize(moveDirection);
    }
}
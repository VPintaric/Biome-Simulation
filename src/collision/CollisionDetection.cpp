#include <GL/glm/gtx/norm.hpp>
#include <memory>
#include "CollisionDetection.h"

CollisionDetection::CollisionDetection() {

}

CollisionDetection& CollisionDetection::getInstance() {
    static CollisionDetection instance;
    return instance;
}

CollisionDetection::~CollisionDetection() {

}

std::shared_ptr<CollisionInfo> CollisionDetection::checkCircleCircleCollision(CircleObject &a, CircleObject &b) {
    std::shared_ptr<CollisionInfo> ci = std::make_shared<CollisionInfo>();

    glm::vec2 p1 = a.getPos(), p2 = b.getPos();
    glm::vec2 p2p1 = p1 - p2;
    float r1 = a.getRadius(), r2 = b.getRadius();

    if(glm::length2(p2p1) > (r1 + r2) * (r1 + r2)){
        ci->isCollision = false;
        return ci;
    }

    ci->isCollision = true;
    ci->normal = glm::normalize(p2p1);
    ci->p1 = p1 - r1 * ci->normal;
    ci->p2 = p2 + r2 * ci->normal;
    ci->mtd = ci->p2 - ci->p1;

    return ci;
}
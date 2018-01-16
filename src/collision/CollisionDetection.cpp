#include <GL/glm/gtx/norm.hpp>
#include <memory>
#include "CollisionDetection.h"
#include "../state/Log.h"

CollisionDetection::CollisionDetection() = default;

CollisionDetection& CollisionDetection::getInstance() {
    static CollisionDetection instance;
    return instance;
}

CollisionDetection::~CollisionDetection() = default;

std::shared_ptr<CollisionInfo> CollisionDetection::checkCircleCircleCollision(CircleObject &a, CircleObject &b) {
    auto ci = std::make_shared<CollisionInfo>();

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

std::shared_ptr<CollisionInfo> CollisionDetection::checkCircleHollowCollision(CircleObject &a, HollowCircleObject &b) {
    auto ci = std::make_shared<CollisionInfo>();

    glm::vec2 p1 = a.getPos(), p2 = b.getPos();
    glm::vec2 p2p1 = p1 - p2;

    float r1 = a.getRadius(), r2 = b.getR1();

    if(glm::length(p2p1) + r1 < r2){
        ci->isCollision = false;
        return ci;
    }

    ci->isCollision = true;
    ci->normal = glm::normalize(-p2p1);
    ci->p1 = p1 - r1 * ci->normal;
    ci->p2 = p2 - r2 * ci->normal;
    ci->mtd = ci->p2 - ci->p1;

    return ci;
}
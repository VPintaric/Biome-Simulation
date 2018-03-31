#include "collision/CollisionDetection.h"
#include "glm/gtx/norm.hpp"
#include "state/Log.h"

CollisionDetection::CollisionDetection() {
    Log().Get(logDEBUG) << "Created new collision detection instance";
};

CollisionDetection& CollisionDetection::getInstance() {
    static CollisionDetection instance;
    return instance;
}

CollisionDetection::~CollisionDetection() = default;

std::shared_ptr<CollisionInfo> CollisionDetection::checkCircleCircleCollision(CircleObject &a, CircleObject &b, bool forceGetInfo) {
    auto ci = std::make_shared<CollisionInfo>();

    glm::vec2 p1 = a.getPos(), p2 = b.getPos();
    glm::vec2 p2p1 = p1 - p2;
    float r1 = a.getRadius(), r2 = b.getRadius();

    if(glm::length2(p2p1) > (r1 + r2) * (r1 + r2)){
        ci->isCollision = false;
        if(!forceGetInfo){
            return ci;
        }
    } else {
        ci->isCollision = true;
    }

    ci->normal = glm::normalize(p2p1);
    ci->p1 = p1 - r1 * ci->normal;
    ci->p2 = p2 + r2 * ci->normal;
    ci->mtd = ci->p2 - ci->p1;

    return ci;
}

std::shared_ptr<CollisionInfo> CollisionDetection::checkCircleHollowCollision(CircleObject &a, HollowCircleObject &b, bool forceGetInfo) {
    auto ci = std::make_shared<CollisionInfo>();

    glm::vec2 p1 = a.getPos(), p2 = b.getPos();
    glm::vec2 p2p1 = p1 - p2;

    float r1 = a.getRadius(), r2 = b.getR1();

    if(glm::length(p2p1) + r1 < r2){
        ci->isCollision = false;
        if(!forceGetInfo){
            return ci;
        }
    } else {
        ci->isCollision = true;
    }

    ci->normal = glm::normalize(-p2p1);
    ci->p1 = p1 - r1 * ci->normal;
    ci->p2 = p2 - r2 * ci->normal;
    ci->mtd = ci->p2 - ci->p1;

    return ci;
}

bool CollisionDetection::pointInCircle(const glm::vec2 &p, CircleObject &o) {
    return glm::length2(p - o.getPos()) <= o.getRadius() * o.getRadius();
}

bool CollisionDetection::pointInHollowCircle(const glm::vec2 &p, HollowCircleObject &o) {
    float d2 = glm::length2(p - o.getPos());
    return d2 <= o.getR2() * o.getR2() && d2 >= o.getR1() * o.getR1();
}
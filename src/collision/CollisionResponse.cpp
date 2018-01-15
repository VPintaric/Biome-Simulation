#include "CollisionResponse.h"

CollisionResponse::CollisionResponse() {

}

CollisionResponse::~CollisionResponse() {

}

CollisionResponse& CollisionResponse::getInstance() {
    static CollisionResponse instance;
    return instance;
}

void CollisionResponse::doCollisionResponse(CircleObject &a, CircleObject &b, std::shared_ptr<CollisionInfo> ci) {
    if(!ci->isCollision){
        return;
    }

    float m1 = a.getRMass(), m2 = b.getRMass();
    glm::vec2 p1 = a.getPos(), p2 = b.getPos();

    a.setPos(p1 + m1 * (m1 + m2) * ci->mtd);
    b.setPos(p2 - m2 * (m1 + m2) * ci->mtd);
}
#include "collision/CollisionResponse.h"
#include "helpers/MathHelpers.h"
#include "state/Log.h"

CollisionResponse::CollisionResponse() {

}

CollisionResponse::~CollisionResponse() {

}

CollisionResponse& CollisionResponse::getInstance() {
    static CollisionResponse instance;
    return instance;
}

void CollisionResponse::doCollisionResponse(Object &a, Object &b, std::shared_ptr<CollisionInfo> ci) {
    const float REST_FACTOR = 0.1f;
    const float FRICTION_FACTOR = 1.f;

    if(!ci->isCollision){
        return;
    }

    float m1 = a.getRMass(), m2 = b.getRMass();
    glm::vec2 p1 = a.getPos(), p2 = b.getPos();

    glm::vec2 transl1 = m1 * (m1 + m2) * ci->mtd;
    glm::vec2 transl2 = m2 * (m1 + m2) * ci->mtd;

    p1 += transl1;
    p2 -= transl2;

    a.setPos(p1);
    b.setPos(p2);

    glm::vec2 r1 = ci->p1 + transl1 - p1;
    glm::vec2 r2 = ci->p2 - transl2 - p2;

    glm::vec2 v1 = a.getVelocity() + Math::perp(r1) * a.getAngleVel();
    glm::vec2 v2 = b.getVelocity() + Math::perp(r2) * b.getAngleVel();

    glm::vec2 v = v1 - v2;

    if(glm::dot(v, ci->normal) >= 0.f){
        return;
    }

    float crossR1N = Math::cross(r1, ci->normal);
    float crossR2N = Math::cross(r2, ci->normal);
    float coll_impulse = glm::dot(v, ci->normal) / ( (a.getRMass() + b.getRMass())
                                                     + crossR1N * crossR1N * a.getRInertia()
                                                     + crossR2N * crossR2N * b.getRInertia());

    glm::vec2 vt = v - glm::dot(v, ci->normal) * ci->normal;
    glm::vec2 fn = glm::normalize(-vt);
    float crossR1fn = Math::cross(r1, fn);
    float crossR2fn = Math::cross(r2, fn);
    float friction_impulse = glm::dot(v, fn) / ( (a.getRMass() + b.getRMass())
                                                     + crossR1fn * crossR1fn * a.getRInertia()
                                                     + crossR2fn * crossR2fn * b.getRInertia());

    if(std::fabs(friction_impulse) > std::fabs(coll_impulse * FRICTION_FACTOR)){
        friction_impulse = coll_impulse * FRICTION_FACTOR;
    }

    glm::vec2 impulse = (coll_impulse * -(1.f + REST_FACTOR)) * ci->normal - friction_impulse * fn;

    a.setVelocity(a.getVelocity() + a.getRMass() * impulse);
    b.setVelocity(b.getVelocity() - b.getRMass() * impulse);

    a.setAngleVel(a.getAngleVel() + Math::cross(r1, impulse) * a.getRInertia());
    b.setAngleVel(b.getAngleVel() - Math::cross(r2, impulse) * b.getRInertia());
}
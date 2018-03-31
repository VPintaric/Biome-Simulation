#ifndef BIOME_SIMULATION_COLLISIONDETECTION_H
#define BIOME_SIMULATION_COLLISIONDETECTION_H


#include <memory>
#include "CollisionInfo.h"
#include "objects/CircleObject.h"
#include "objects/HollowCircleObject.h"

class CollisionDetection {
public:
    static CollisionDetection& getInstance();
    ~CollisionDetection();

    std::shared_ptr<CollisionInfo> checkCircleCircleCollision(CircleObject &a, CircleObject &b, bool forceGetInfo=false);
    std::shared_ptr<CollisionInfo> checkCircleHollowCollision(CircleObject &a, HollowCircleObject &b, bool forceGetInfo=false);

    bool pointInCircle(const glm::vec2& p, CircleObject &o);
    bool pointInHollowCircle(const glm::vec2& p, HollowCircleObject &o);

private:
    CollisionDetection();
};


#endif //BIOME_SIMULATION_COLLISIONDETECTION_H

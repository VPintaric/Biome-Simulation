#ifndef BIOME_SIMULATION_COLLISIONDETECTION_H
#define BIOME_SIMULATION_COLLISIONDETECTION_H


#include <memory>
#include "CollisionInfo.h"
#include "../objects/CircleObject.h"

class CollisionDetection {
public:
    static CollisionDetection& getInstance();
    ~CollisionDetection();

    std::shared_ptr<CollisionInfo> checkCircleCircleCollision(CircleObject &a, CircleObject &b);

private:
    CollisionDetection();
};


#endif //BIOME_SIMULATION_COLLISIONDETECTION_H

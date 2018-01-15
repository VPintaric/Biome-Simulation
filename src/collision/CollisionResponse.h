#ifndef BIOME_SIMULATION_COLLISIONRESPONSE_H
#define BIOME_SIMULATION_COLLISIONRESPONSE_H


#include <memory>
#include "../objects/CircleObject.h"
#include "CollisionInfo.h"

class CollisionResponse {
public:
    ~CollisionResponse();
    static CollisionResponse& getInstance();

    void doCollisionResponse(CircleObject &a, CircleObject &b, std::shared_ptr<CollisionInfo> ci);

private:
    CollisionResponse();
};


#endif //BIOME_SIMULATION_COLLISIONRESPONSE_H

#ifndef BIOME_SIMULATION_COLLISIONINFO_H
#define BIOME_SIMULATION_COLLISIONINFO_H


#include "glm/glm.hpp"

class CollisionInfo {
public:
    bool isCollision;

    glm::vec2 p1, p2;
    glm::vec2 mtd, normal;
};


#endif //BIOME_SIMULATION_COLLISIONINFO_H

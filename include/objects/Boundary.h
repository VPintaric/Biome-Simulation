#ifndef BIOME_SIMULATION_BOUNDARIES_H
#define BIOME_SIMULATION_BOUNDARIES_H

#include <memory>
#include "HollowCircleObject.h"
#include "rendering/Model.h"

class Boundary : public HollowCircleObject {
public:
    Boundary();
    ~Boundary();

    void draw();

    const glm::vec4 &getColor() const;

    void setColor(const glm::vec4 &color);


private:
    glm::vec4 color;

    std::shared_ptr<Model> model;
};


#endif //BIOME_SIMULATION_BOUNDARIES_H

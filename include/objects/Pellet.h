#ifndef BIOME_SIMULATION_PELLET_H
#define BIOME_SIMULATION_PELLET_H


#include <objects/CircleObject.h>
#include <rendering/Model.h>
#include <memory>

class Pellet : public CircleObject {
private:
    glm::vec4 color;
    std::shared_ptr<Model> model;
    float life;

public:
    Pellet();

    void draw();

    float getLife() const;

    void setLife(float life);

    const glm::vec4 &getColor() const;

    void setColor(const glm::vec4 &color);
};


#endif //BIOME_SIMULATION_PELLET_H
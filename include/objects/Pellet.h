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
    bool isFoodAttr;

public:
    Pellet(bool isFood);

    void draw();

    float getLife() const;

    void setLife(float life);

    const glm::vec4 &getColor() const;

    void setColor(const glm::vec4 &color);

    bool isFood() const;
};


#endif //BIOME_SIMULATION_PELLET_H

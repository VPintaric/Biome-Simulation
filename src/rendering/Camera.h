#ifndef BIOME_SIMULATION_CAMERA_H
#define BIOME_SIMULATION_CAMERA_H


#include <GL/glm/glm.hpp>

class Camera {
public:
    enum Direction{
        UP, DOWN, LEFT, RIGHT, IN, OUT
    };

    static Camera &getInstance();
    ~Camera();

    void update(float deltaT);

    void startMoveInDirection(Direction dir);

    void stopMoveInDirection(Direction dir);

    const glm::vec3 &getPos() const;

    void setPos(const glm::vec3 &pos);

    float getSpeed() const;

    void setSpeed(float speed);

    const glm::vec3 &getMoveDirection() const;

private:
    Camera();

    glm::vec3 pos, moveDirection;
    float speed;
};


#endif //BIOME_SIMULATION_CAMERA_H

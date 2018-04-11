#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <constants/SimulationConstants.h>
#include <state/State.h>
#include <collision/CollisionDetection.h>
#include <glm/gtx/vector_angle.hpp>
#include <rendering/Renderer.h>
#include <glm/gtc/type_ptr.hpp>
#include <state/Log.h>
#include "minion/senses/simple/SimpleMinionSenses.h"

SimpleMinionSenses::SimpleMinionSenses(std::weak_ptr<Minion> minion, float maxSenseDistance, int nSightLines)
        : maxSenseDistance(maxSenseDistance), nSightLines(nSightLines), minion(std::move(minion)) {
    setRadius(maxSenseDistance);
    createNewSightLinesModel();
}

SimpleMinionSenses::~SimpleMinionSenses() = default;

std::shared_ptr<Minion> SimpleMinionSenses::lockMinion() {
    auto locked = minion.lock();
    if(!locked){
        Log().Get(logERROR) << "Can't get a lock on minion pointer";
        return nullptr;
    }
    return locked;
}

void SimpleMinionSenses::createNewSightLinesModel() {
    std::vector<GLfloat> v;
    v.reserve(static_cast<unsigned long>(6 * nSightLines));

    std::vector<GLfloat> c;
    c.reserve(static_cast<unsigned long>(6 * nSightLines));

    glm::vec2 p(1.f, 0.f);
    float angleResolution = glm::pi<float>() / (nSightLines - 1);
    for(int i = 0; i < nSightLines; i++){
        v.push_back(0.f); v.push_back(0.f); v.push_back(0.f);
        c.push_back(1.f); c.push_back(1.f); c.push_back(1.f);

        v.push_back(p.x); v.push_back(p.y); v.push_back(0.f);
        c.push_back(1.f); c.push_back(1.f); c.push_back(1.f);

        p = glm::rotate(p, -angleResolution);
    }

    sightLinesModel = std::make_unique<ColorModel>(v, c, GL_LINES, GL_DYNAMIC_DRAW);
}

void SimpleMinionSenses::setMaxSenseDistance(float dist) {
    auto minion = lockMinion();

    maxSenseDistance = dist;
    setRadius(dist + minion->getObject()->getRadius());
}

void SimpleMinionSenses::setNSightLines(int n) {
    nSightLines = n;
    createNewSightLinesModel();
}

void SimpleMinionSenses::setSightLineColor(int idx, glm::vec3 color) {
    sightLinesModel->colors[idx * 6] = color.r;
    sightLinesModel->colors[idx * 6 + 1] = color.g;
    sightLinesModel->colors[idx * 6 + 2] = color.b;
    sightLinesModel->colors[idx * 6 + 3] = color.r;
    sightLinesModel->colors[idx * 6 + 4] = color.g;
    sightLinesModel->colors[idx * 6 + 5] = color.b;
}

int SimpleMinionSenses::getDataSize() {
    return 5 * nSightLines + 5;
}

/*
 * Returned vector represents info received from each of the sight lines.
 * Each sight line gives info about distance, color and type of the detected object.
 * If no object is detected on the sight line distance is set to maximum range of senses, color is set to black and
 * type is set to 0.f.
 * First elements are data from the first sight line on the hard right side of the minion, next sight line
 * is a bit to the left (amount of data given depends on the number of sight lines).
 * Last elements of the vector in order are:
 *      - distance of an object behind the minion (half circle behind the minion)
 *      - current life of the minion
 *      - norm of current velocity of the minion
 *      - angle between current velocity vector and the vector pointing forward from the minion
 *      - current angle velocity of the minion
 */
std::vector<float> SimpleMinionSenses::gatherData(float deltaT) {
    auto minion = lockMinion();

    State& state = State::getInstance();
    CollisionDetection cd = CollisionDetection::getInstance();

    std::vector<std::shared_ptr<Minion>> minionsInRange;
    std::vector<float> data;
    float nearestBehind = getRadius() - minion->getObject()->getRadius();

    for(auto m : state.getMinions()){
        // don't check collision of this minion's senses with itself
        if(m == minion){
            continue;
        }
        auto ci = cd.checkCircleCircleCollision(*this, *m->getObject());

        if(ci->isCollision){
            float angle = glm::orientedAngle(-ci->normal, getFront());
            if(angle < -glm::half_pi<float>() || angle > glm::half_pi<float>()){
                float dist = getRadius() - minion->getObject()->getRadius() - glm::length(ci->mtd);
                nearestBehind = std::min(dist, nearestBehind);
            }
            minionsInRange.push_back(m);
        }
    }

    bool boundaryInRange = false;
    auto ci = cd.checkCircleHollowCollision(*this, *state.getBoundary());
    if(ci->isCollision){
        float angle = glm::orientedAngle(-ci->normal, getFront());
        if(angle < -glm::half_pi<float>() || angle > glm::half_pi<float>()){
            float dist = getRadius() - minion->getObject()->getRadius() - glm::length(ci->mtd);
            nearestBehind = std::min(dist, nearestBehind);
        }
        boundaryInRange = true;
    }

    float step = maxSenseDistance / N_SIGHT_LINE_CHECK_POINTS;
    float angleResolution = glm::pi<float>() / (nSightLines - 1);
    glm::vec2 s = glm::rotate(minion->getObject()->getFront(), glm::half_pi<float>());
    glm::vec2 p = getPos() + minion->getObject()->getRadius() * s;
    for(int i = 0; i < nSightLines; i++){
        float distance = 0.f;
        bool detectedObject = false;
        for(int j = 0; j < N_SIGHT_LINE_CHECK_POINTS && !detectedObject; j++){
            for(const auto &m : minionsInRange){
                if(cd.pointInCircle(p, *m->getObject())){
                    detectedObject = true;

                    data.push_back(distance);
                    data.push_back(m->isDead() ? DEAD_MINION_TYPE : ALIVE_MINION_TYPE);
                    auto color = m->getObject()->getSkinColor();
                    data.push_back(color.r);
                    data.push_back(color.g);
                    data.push_back(color.b);

                    setSightLineColor(i, color);

                    break;
                }
            }
            if(!detectedObject && boundaryInRange && cd.pointInHollowCircle(p, *state.getBoundary())){
                detectedObject = true;

                data.push_back(distance);
                data.push_back(BOUNDARY_TYPE);
                auto color = state.getBoundary()->getColor();
                data.push_back(color.r);
                data.push_back(color.g);
                data.push_back(color.b);

                setSightLineColor(i, color);
            }

            p += step * s;
            distance += step;
        }
        if(!detectedObject){
            data.push_back(distance);
            data.push_back(NOTHING_TYPE);
            data.push_back(0.f);
            data.push_back(0.f);
            data.push_back(0.f);

            setSightLineColor(i, glm::vec3(1.f, 1.f, 1.f));
        }
        s = glm::rotate(s, -angleResolution);
        p = getPos() + minion->getObject()->getRadius() * s;
    }
    sightLinesModel->updateColorBuffer();

    data.push_back(nearestBehind);
    data.push_back(minion->getLife());
    data.push_back(glm::length(minion->getObject()->getVelocity()));
    data.push_back(glm::orientedAngle(minion->getObject()->getFront(), minion->getObject()->getVelocity()));
    data.push_back(minion->getObject()->getAngleVel());

    return data;
}

void SimpleMinionSenses::draw() {
    glm::vec2 pos = getPos();
    float angle = getAngle();
    float scale = getRadius();

    Renderer &r = Renderer::getInstance();

    GLuint shaderId = r.getActiveShaderProgram()->getId();

    r.pushMatrix(Renderer::Matrix::MODEL);
    r.transform(pos.x, pos.y, 1.f,
                angle, scale, scale);

    glUniform4fv(glGetUniformLocation(shaderId, "color_mod"), 1, glm::value_ptr(glm::vec4(1.f, 1.f, 1.f, COLOR_ALPHA)));
    r.getModel(SimConst::HOLLOW_CIRCLE_MODEL_NAME)->draw();
    sightLinesModel->draw();

    r.popMatrix(Renderer::Matrix::MODEL);
}

float SimpleMinionSenses::getMaxSenseDistance() {
    return maxSenseDistance;
}

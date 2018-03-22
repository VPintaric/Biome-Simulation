#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <state/State.h>
#include <glm/gtx/vector_angle.hpp>
#include <collision/CollisionDetection.h>
#include <state/Log.h>
#include <rendering/Renderer.h>
#include <glm/gtc/type_ptr.hpp>
#include <constants/SimulationConstants.h>

MinionSenses::MinionSenses() = default;
MinionSenses::~MinionSenses() = default;

void MinionSenses::draw() {
    glm::vec2 pos = getPos();
    float angle = getAngle();
    float scale = getRadius();

    Renderer &r = Renderer::getInstance();

    GLuint shaderId = r.getActiveShaderProgram()->getId();

    r.pushMatrix(Renderer::Matrix::MODEL);
    r.transform(pos.x, pos.y, 1.f,
                angle, scale, scale);

    glUniform4fv(glGetUniformLocation(shaderId, "color_mod"), 1, glm::value_ptr(glm::vec4(1.f, 1.f, 1.f, 0.05f)));
    r.getModel(SimConst::MINION_CIRCLE_MODEL_NAME)->draw();

    r.popMatrix(Renderer::Matrix::MODEL);
}

void MinionSenses::gatherData(std::shared_ptr<MinionObject> minionObject) {
    State& state = State::getInstance();
    CollisionDetection cd = CollisionDetection::getInstance();

    this->setPos(minionObject->getPos());
    this->setAngle(minionObject->getAngle());
    this->setRadius(minionObject->getRadius() + MAX_SENSE_DISTANCE);

    for(auto minion : state.getMinions()){
        // don't check collision of this minion's senses with itself
        if(minion->getObject() == minionObject){
            continue;
        }
        auto ci = cd.checkCircleCircleCollision(*this, *minion->getObject());
        auto s = std::make_shared<MinionSenses::SenseData>();

        if(ci->isCollision){
            s->dist = this->getRadius() - minion->getObject()->getRadius() - glm::length(ci->mtd);
            s->color = minion->getObject()->getSkinColor();
            s->angle = glm::orientedAngle(-ci->normal, minionObject->getFront());

            senseDatas.push_back(s);
        }
    }
}

std::shared_ptr<MinionSenses::SenseData> MinionSenses::popSenseData() {
    if(senseDatas.empty()){
        return nullptr;
    }

    auto ret = senseDatas.back();
    senseDatas.pop_back();
    return ret;
}

void MinionSenses::clearData() {
    senseDatas.clear();
}

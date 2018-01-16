#include <GL/glew.h>
#include <vector>
#include <memory>
#include "../state/Log.h"
#include "../rendering/Model.h"
#include "../rendering/ColorModel.h"
#include "../rendering/Renderer.h"
#include "../constants/SimulationConstants.h"
#include "MinionModelCreator.h"

void MinionModelCreator::createMinionModel() {
    Log().Get(logDEBUG) << "Creating basic minion model (triangle)";

    Renderer &r = Renderer::getInstance();

    std::shared_ptr<Model> m = std::make_shared<ColorModel>(TRIANGLE_VERTICES, TRIANGLE_COLORS);
    r.addNewModel(SimConst::MINION_BODY_MODEL_NAME, m);

    m = std::make_shared<ColorModel>(CIRCLE_VERTICES, CIRCLE_COLORS);
    r.addNewModel(SimConst::MINION_CIRCLE_MODEL_NAME, m);
}
#include <GL/glew.h>
#include <vector>
#include <memory>
#include "state/Log.h"
#include "rendering/Model.h"
#include "rendering/ColorModel.h"
#include "rendering/Renderer.h"
#include "constants/SimulationConstants.h"
#include "helpers/ModelCreator.h"

void ModelCreator::createModels() {
    Log().Get(logDEBUG) << "Creating models...";

    Renderer &r = Renderer::getInstance();

    std::shared_ptr<Model> m = std::make_shared<ColorModel>(TRIANGLE_VERTICES, TRIANGLE_COLORS);
    r.addNewModel(SimConst::MINION_BODY_MODEL_NAME, m);

    m = std::make_shared<ColorModel>(TEETH_CIRCLE_VERTICES, TEETH_CIRCLE_COLORS);
    r.addNewModel(SimConst::MINION_CIRCLE_MODEL_NAME, m);

    m = std::make_shared<ColorModel>(HOLLOW_CIRCLE_VERTICES, HOLLOW_CIRCLE_COLORS);
    r.addNewModel(SimConst::HOLLOW_CIRCLE_MODEL_NAME, m);

    m = std::make_shared<ColorModel>(FULL_CIRCLE_VERTICES, FULL_CIRCLE_COLORS, GL_POLYGON);
    r.addNewModel(SimConst::FULL_CIRCLE_MODEL_NAME, m);
}
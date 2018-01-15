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

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;

    vertices.push_back((GLfloat) -1.0); vertices.push_back((GLfloat) 1.0);  vertices.push_back((GLfloat) 0.0);
    vertices.push_back((GLfloat) 1.0);  vertices.push_back((GLfloat) 1.0);  vertices.push_back((GLfloat) 0.0);
    vertices.push_back((GLfloat) 0.0);  vertices.push_back((GLfloat) -1.0); vertices.push_back((GLfloat) 0.0);

    colors.push_back((GLfloat) 0.0); colors.push_back((GLfloat) 0.2); colors.push_back((GLfloat) 0.0);
    colors.push_back((GLfloat) 0.0); colors.push_back((GLfloat) 0.2); colors.push_back((GLfloat) 0.0);
    colors.push_back((GLfloat) 0.75); colors.push_back((GLfloat) 0.2); colors.push_back((GLfloat) 0.0);

    std::shared_ptr<Model> m = std::make_shared<ColorModel>(vertices, colors);
    Renderer::getInstance().addNewModel(SimConst::MINION_MODEL_NAME, m);
}
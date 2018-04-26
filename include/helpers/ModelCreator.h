#ifndef BIOME_SIMULATION_MINIONMODELCREATOR_H
#define BIOME_SIMULATION_MINIONMODELCREATOR_H


namespace ModelCreator {
    void createModels();

    const std::vector<GLfloat> TRIANGLE_VERTICES = {-0.7071f, 0.7071f, 0.f,
                                                    0.7071f, 0.7071f, 0.f,
                                                    0.f, -1.f, 0.f};

    const std::vector<GLfloat> TRIANGLE_COLORS(TRIANGLE_VERTICES.size(), 1.f);

    const std::vector<GLfloat> TEETH_CIRCLE_VERTICES = {
#include "constants/teeth_circle_vertices.txt"
    };

    const std::vector<GLfloat> TEETH_CIRCLE_COLORS(TEETH_CIRCLE_VERTICES.size(), 1.f);

    const std::vector<GLfloat> HOLLOW_CIRCLE_VERTICES = {
#include "constants/hollow_circle_vertices.txt"
    };

    const std::vector<GLfloat> HOLLOW_CIRCLE_COLORS(HOLLOW_CIRCLE_VERTICES.size(), 1.f);

    const std::vector<GLfloat> FULL_CIRCLE_VERTICES = {
#include "constants/full_circle_vertices.txt"
    };

    const std::vector<GLfloat> FULL_CIRCLE_COLORS(FULL_CIRCLE_VERTICES.size(), 1.f);
};


#endif //BIOME_SIMULATION_MINIONMODELCREATOR_H

cmake_minimum_required(VERSION 3.9)
project(Biome_Simulation)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
if(NOT ${SDL2_FOUND})
    message(ERROR " sdl2 not found")
endif()

find_package(GLEW REQUIRED)
if(NOT ${GLEW_FOUND})
    message(ERROR " GLEW not found")
endif()

find_package(OpenGL REQUIRED)
if(NOT ${OpenGL_FOUND})
    message(ERROR " OpenGL not found")
endif()

include_directories(${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OpenGL_INCLUDE_DIRS} external include)

add_executable(Biome_Simulation
        include/constants/SimulationConstants.h
        include/constants/WindowConstants.h
        src/helpers/GLHelpers.cpp
        include/helpers/GLHelpers.h
        include/helpers/MathHelpers.h
        src/minion/object/MinionObject.cpp
        include/minion/object/MinionObject.h
        src/rendering/ColorModel.cpp
        include/rendering/ColorModel.h
        src/rendering/Model.cpp
        include/rendering/Model.h
        src/rendering/Renderer.cpp
        include/rendering/Renderer.h
        src/shader_handling/ProgramLoader.cpp
        include/shader_handling/ProgramLoader.h
        src/shader_handling/ShaderLoader.cpp
        include/shader_handling/ShaderLoader.h
        src/state/Display.cpp
        include/state/Display.h
        include/state/Log.h
        src/state/State.cpp
        include/state/State.h
        src/main.cpp
        src/collision/CollisionDetection.cpp
        include/collision/CollisionDetection.h
        include/collision/CollisionInfo.h
        src/objects/CircleObject.cpp
        include/objects/CircleObject.h
        src/helpers/ModelCreator.cpp
        include/helpers/ModelCreator.h
        src/collision/CollisionResponse.cpp
        include/collision/CollisionResponse.h
        src/rendering/Camera.cpp
        include/rendering/Camera.h
        src/objects/Object.cpp
        include/objects/Object.h
        src/objects/HollowCircleObject.cpp
        include/objects/HollowCircleObject.h
        src/objects/Boundary.cpp
        include/objects/Boundary.h
        src/minion/Minion.cpp
        src/minion/Minion.cpp
        include/minion/Minion.h
        src/minion/senses/MinionSenses.cpp
        include/minion/senses/MinionSenses.h
        src/minion/controllers/MinionController.cpp
        include/minion/controllers/MinionController.h
        src/helpers/MathHelpers.cpp
        src/minion/controllers/RandomController.cpp
        include/minion/controllers/RandomController.h
        src/minion/controllers/CowardController.cpp
        include/minion/controllers/CowardController.h
        src/minion/controllers/BraveController.cpp
        include/minion/controllers/BraveController.h
        src/minion/factories/MinionGenerator.cpp
        include/minion/factories/MinionGenerator.h
        src/minion/controllers/NullController.cpp
        include/minion/controllers/NullController.h
        src/minion/controllers/NeuralNetController.cpp
        include/minion/controllers/NeuralNetController.h)

message(ERROR " ${SDL2_INCLUDE_DIRS}")
message(ERROR " ${GLEW_INCLUDE_DIRS}")
message(ERROR " ${OpenGL_INCLUDE_DIRS}")

message(ERROR " ${SDL2_LIBRARIES}")
message(ERROR " ${GLEW_LIBRARIES}")
message(ERROR " ${OpenGL_LIBRARIES}")

target_link_libraries(Biome_Simulation ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OpenGL_LIBRARIES} "-lGL")
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

include_directories(${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OpenGL_INCLUDE_DIRS} external)

add_executable(Biome_Simulation
        src/constants/SimulationConstants.h
        src/constants/WindowConstants.h
        src/helpers/GLHelpers.cpp
        src/helpers/GLHelpers.h
        src/helpers/MathHelpers.h
        src/minion/object/MinionObject.cpp
        src/minion/object/MinionObject.h
        src/rendering/ColorModel.cpp
        src/rendering/ColorModel.h
        src/rendering/Model.cpp
        src/rendering/Model.h
        src/rendering/Renderer.cpp
        src/rendering/Renderer.h
        src/shader_handling/ProgramLoader.cpp
        src/shader_handling/ProgramLoader.h
        src/shader_handling/ShaderLoader.cpp
        src/shader_handling/ShaderLoader.h
        src/state/Display.cpp
        src/state/Display.h
        src/state/Log.h
        src/state/State.cpp
        src/state/State.h
        src/main.cpp
        src/collision/CollisionDetection.cpp
        src/collision/CollisionDetection.h
        src/collision/CollisionInfo.h
        src/objects/CircleObject.cpp
        src/objects/CircleObject.h
        src/helpers/ModelCreator.cpp
        src/helpers/ModelCreator.h
        src/collision/CollisionResponse.cpp
        src/collision/CollisionResponse.h
        src/rendering/Camera.cpp
        src/rendering/Camera.h
        src/objects/Object.cpp
        src/objects/Object.h
        src/objects/HollowCircleObject.cpp
        src/objects/HollowCircleObject.h
        src/objects/Boundary.cpp
        src/objects/Boundary.h
        src/minion/Minion.cpp
        src/minion/Minion.cpp
        src/minion/Minion.h
        src/minion/senses/MinionSenses.cpp
        src/minion/senses/MinionSenses.h
        src/minion/controllers/MinionController.cpp
        src/minion/controllers/MinionController.h
        src/helpers/MathHelpers.cpp src/minion/controllers/RandomController.cpp src/minion/controllers/RandomController.h src/minion/controllers/CowardController.cpp src/minion/controllers/CowardController.h src/minion/controllers/BraveController.cpp src/minion/controllers/BraveController.h src/minion/factories/MinionGenerator.cpp src/minion/factories/MinionGenerator.h src/minion/controllers/NullController.cpp src/minion/controllers/NullController.h src/minion/controllers/NeuralNetController.cpp src/minion/controllers/NeuralNetController.h)

message(ERROR " ${SDL2_INCLUDE_DIRS}")
message(ERROR " ${GLEW_INCLUDE_DIRS}")
message(ERROR " ${OpenGL_INCLUDE_DIRS}")

message(ERROR " ${SDL2_LIBRARIES}")
message(ERROR " ${GLEW_LIBRARIES}")
message(ERROR " ${OpenGL_LIBRARIES}")

target_link_libraries(Biome_Simulation ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OpenGL_LIBRARIES} "-lGL")
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
        src/minion/controllers/neural_net/NeuralNetController.cpp
        include/minion/controllers/neural_net/NeuralNetController.h
        include/minion/factories/MinionGenerator.h
        src/minion/factories/neuralnet/NeuralNetMinionGenerator.cpp
        include/minion/factories/neuralnet/NeuralNetMinionGenerator.h
        src/minion/controllers/neural_net/NeuralNet.cpp
        include/minion/controllers/neural_net/NeuralNet.h
        src/minion/senses/simple/SimpleMinionSenses.cpp
        include/minion/senses/simple/SimpleMinionSenses.h
        include/minion/selection/Selection.h
        src/persistence/Persistence.cpp
        include/persistence/Persistence.h
        include/persistence/JSONPersistable.h
        src/config/Config.cpp
        include/config/Config.h
        include/config/JSONConfigurable.h
        src/minion/factories/neuralnet/crossover/NeuralNetCrossover.cpp
        include/minion/factories/neuralnet/crossover/NeuralNetCrossover.h
        src/minion/factories/neuralnet/mutation/NeuralNetMutation.cpp
        include/minion/factories/neuralnet/mutation/NeuralNetMutation.h
        src/minion/factories/neuralnet/crossover/ArithmeticAverageCrossover.cpp
        include/minion/factories/neuralnet/crossover/ArithmeticAverageCrossover.h
        src/minion/factories/neuralnet/mutation/GaussNoiseMutation.cpp
        include/minion/factories/neuralnet/mutation/GaussNoiseMutation.h
        src/minion/factories/neuralnet/mutation/SparseResetMutation.cpp
        include/minion/factories/neuralnet/mutation/SparseResetMutation.h
        src/minion/factories/neuralnet/crossover/InterleaveLayersCrossover.cpp
        include/minion/factories/neuralnet/crossover/InterleaveLayersCrossover.h
        src/minion/factories/neuralnet/crossover/PickRandomCrossover.cpp
        include/minion/factories/neuralnet/crossover/PickRandomCrossover.h
        include/minion/fitness/Fitness.h
        src/minion/fitness/TimeLivedFitness.cpp
        include/minion/fitness/TimeLivedFitness.h
        src/minion/fitness/ActivityFitness.cpp
        include/minion/fitness/ActivityFitness.h
        src/minion/selection/RoulleteWheelSelection.cpp
        include/minion/selection/RoulleteWheelSelection.h
        src/helpers/RNG.cpp
        include/helpers/RNG.h include/minion/crossover_operators/MinionCrossover.h
        include/minion/mutation_operators/MinionMutation.h
        src/minion/crossover_operators/PickBetterMinionCrossover.cpp
        include/minion/crossover_operators/PickBetterMinionCrossover.h
        include/copyable/Copyable.h
        src/minion/crossover_operators/CustomMinionCrossover.cpp
        include/minion/crossover_operators/CustomMinionCrossover.h
        include/minion/crossover_operators/controller/MinionControllerCrossover.h
        include/minion/crossover_operators/object/MinionObjectCrossover.h
        include/minion/crossover_operators/senses/MinionSensesCrossover.h
        include/minion/mutation_operators/controller/MinionControllerMutation.h
        include/minion/mutation_operators/object/MinionObjectMutation.h
        include/minion/mutation_operators/senses/MinionSensesMutation.h
        src/minion/crossover_operators/PickRandomMinionCrossover.cpp
        include/minion/crossover_operators/PickRandomMinionCrossover.h
        src/minion/mutation_operators/DoNothingMinionMutation.cpp
        include/minion/mutation_operators/DoNothingMinionMutation.h
        src/minion/mutation_operators/CustomMinionMutation.cpp
        include/minion/mutation_operators/CustomMinionMutation.h
        src/minion/crossover_operators/senses/PickBetterSensesCrossover.cpp
        include/minion/crossover_operators/senses/PickBetterSensesCrossover.h
        src/minion/crossover_operators/object/PickBetterObjectCrossover.cpp
        include/minion/crossover_operators/object/PickBetterObjectCrossover.h
        src/minion/crossover_operators/controller/PickBetterControllerCrossover.cpp
        include/minion/crossover_operators/controller/PickBetterControllerCrossover.h
        src/minion/crossover_operators/senses/FitnessWeightedAverageSensesCrossover.cpp
        include/minion/crossover_operators/senses/FitnessWeightedAverageSensesCrossover.h
        src/minion/crossover_operators/object/FitnessWeightedAverageObjectCrossover.cpp
        include/minion/crossover_operators/object/FitnessWeightedAverageObjectCrossover.h
        src/minion/crossover_operators/controller/neural_net/NeuralNetControllerCrossover.cpp
        include/minion/crossover_operators/controller/neural_net/NeuralNetControllerCrossover.h
        src/minion/mutation_operators/senses/GaussNoiseSensesMutation.cpp
        include/minion/mutation_operators/senses/GaussNoiseSensesMutation.h
        src/minion/mutation_operators/object/GaussNoiseObjectMutation.cpp
        include/minion/mutation_operators/object/GaussNoiseObjectMutation.h
        src/minion/mutation_operators/controller/neural_net/NeuralNetControllerMutation.cpp
        include/minion/mutation_operators/controller/neural_net/NeuralNetControllerMutation.h
        src/minion/mutation_operators/controller/neural_net/NeuralNetGaussNoiseWeightsMutation.cpp
        include/minion/mutation_operators/controller/neural_net/NeuralNetGaussNoiseWeightsMutation.h src/objects/Pellet.cpp include/objects/Pellet.h src/minion/mutation_operators/controller/neural_net/NeuralNetSparseResetWeightsMutation.cpp include/minion/mutation_operators/controller/neural_net/NeuralNetSparseResetWeightsMutation.h src/minion/crossover_operators/controller/neural_net/NeuralNetSingleLayerCrossover.cpp src/minion/crossover_operators/controller/neural_net/NeuralNetSingleLayerCrossover.h src/minion/controllers/decision_tree/DecisionTreeController.cpp include/minion/controllers/decision_tree/DecisionTreeController.h include/minion/controllers/decision_tree/DTConst.h src/minion/controllers/decision_tree/DecisionTree.cpp include/minion/controllers/decision_tree/DecisionTree.h include/minion/controllers/decision_tree/DTNode.h src/minion/controllers/decision_tree/DTTerminalNode.cpp include/minion/controllers/decision_tree/DTTerminalNode.h src/minion/controllers/decision_tree/DTBranchNode.cpp include/minion/controllers/decision_tree/DTBranchNode.h src/minion/factories/decisiontree/DecisionTreeMinionGenerator.cpp include/minion/factories/decisiontree/DecisionTreeMinionGenerator.h)

message(ERROR " ${SDL2_INCLUDE_DIRS}")
message(ERROR " ${GLEW_INCLUDE_DIRS}")
message(ERROR " ${OpenGL_INCLUDE_DIRS}")

message(ERROR " ${SDL2_LIBRARIES}")
message(ERROR " ${GLEW_LIBRARIES}")
message(ERROR " ${OpenGL_LIBRARIES}")

target_link_libraries(Biome_Simulation ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OpenGL_LIBRARIES} "-lGL -lstdc++fs")
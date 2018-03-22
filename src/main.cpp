#define GLM_FORCE_RADIANS // needed by GLM!

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <chrono>
#include <minion/factories/explicit/ExplicitBehaviourMinionGenerator.h>
#include <minion/factories/neuralnet/NeuralNetMinionGenerator.h>
#include <neuralnet/NeuralNet.h>
#include "state/State.h"

#include "state/Log.h"
#include "constants/WindowConstants.h"
#include "state/Display.h"
#include "shader_handling/ProgramLoader.h"
#include "rendering/Renderer.h"
#include "constants/SimulationConstants.h"
#include "../include/helpers/ModelCreator.h"
#include "rendering/Camera.h"

void init(){
    Log::ReportingLevel() = logDEBUG;

    Log().Get(logDEBUG) << "Initializing SDL2...";
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        Log().Get(logERROR) << "Failed to initialize SDL2!";
        exit(1);
    }

    Log().Get(logDEBUG) << "Creating a window with openGL context...";
    if(Display::getInstance().createWindow(WindowConst::WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, WindowConst::WINDOW_WIDTH, WindowConst::WINDOW_HEIGHT,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE) == nullptr){
        Log().Get(logERROR) << "Failed to create a window!";
        exit(1);
    }

    Log().Get(logDEBUG) << "Setting up GL context attributes...";
    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) < 0 ||
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0) < 0 ||
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) < 0 ||
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0){
        Log().Get(logERROR) << "Failed to set openGL version to 3.0 and core profile!";
        exit(1);
    }

    if(Display::getInstance().attachContext() == nullptr){
        Log().Get(logERROR) << "Failed to attach GL context to window!";
        exit(1);
    }

    Log().Get(logDEBUG) << "Turning on vSync...";
    if(SDL_GL_SetSwapInterval(1) < 0){
        Log().Get(logWARNING) << "vSync not supported!";
    }

    Log().Get(logDEBUG) << "Initializing GLEW...";
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        Log().Get(logERROR) << "Failed to initialize GLEW library!";
        exit(1);
    }


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::shared_ptr<ProgramLoader> shaderProgram =
            std::make_shared<ProgramLoader>("resources/shaders/vertex_shaders/vertex_shader.glsl",
                                            "resources/shaders/fragment_shaders/fragment_shader.glsl");

    Renderer &renderer = Renderer::getInstance();
    renderer.addShaderProgram(WindowConst::DEFAULT_SHADER_NAME, shaderProgram);
    renderer.activateShaderProgram(WindowConst::DEFAULT_SHADER_NAME);

    ModelCreator::createModels();

    renderer.setPerspectiveProjection(glm::pi<float>() / 3.f,
                                      (float)WindowConst::WINDOW_WIDTH / WindowConst::WINDOW_HEIGHT, 0.01f);
    Camera::getInstance().setPos(WindowConst::INIT_CAMERA_POS);
    renderer.identity();


    State &s = State::getInstance();
//    s.setMinionGenerator(std::make_shared<ExplicitBehaviourMinionGenerator>());
    s.setMinionGenerator(std::make_shared<NeuralNetMinionGenerator>());
    s.initBoundary(450.f);
    s.spawnMinions(40);
}

void windowResize(int w, int h){
    Renderer &r = Renderer::getInstance();

    // doesn't work correctly for some reason...
    r.setPerspectiveProjection(glm::pi<float>() / 3.f, (float) w / h, 0.01f);
}

void processInput(State &state) {
    SDL_Event event{};

    Camera &c = Camera::getInstance();
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                Log().Get(logINFO) << "Window resized to " << event.window.data1 << "x" << event.window.data2;
                windowResize(event.window.data1, event.window.data2);
            } else if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                Log().Get(logINFO) << "Window closed";
                state.endProgram();
                if (state.getShouldEndProgram()) {
                    break;
                }
            }
        } else if(event.type == SDL_KEYDOWN && event.key.repeat == 0) {
            switch(event.key.keysym.sym){
                case SDLK_w:
                    c.startMoveInDirection(Camera::UP);
                    break;
                case SDLK_s:
                    c.startMoveInDirection(Camera::DOWN);
                    break;
                case SDLK_a:
                    c.startMoveInDirection(Camera::LEFT);
                    break;
                case SDLK_d:
                    c.startMoveInDirection(Camera::RIGHT);
                    break;
                case SDLK_q:
                    c.startMoveInDirection(Camera::OUT);
                    break;
                case SDLK_e:
                    c.startMoveInDirection(Camera::IN);
                    break;
                default:
                    break;
            }
        } else if(event.type == SDL_KEYUP){
            switch(event.key.keysym.sym){
                case SDLK_w:
                    c.stopMoveInDirection(Camera::UP);
                    break;
                case SDLK_s:
                    c.stopMoveInDirection(Camera::DOWN);
                    break;
                case SDLK_a:
                    c.stopMoveInDirection(Camera::LEFT);
                    break;
                case SDLK_d:
                    c.stopMoveInDirection(Camera::RIGHT);
                    break;
                case SDLK_q:
                    c.stopMoveInDirection(Camera::OUT);
                    break;
                case SDLK_e:
                    c.stopMoveInDirection(Camera::IN);
                    break;
                default:
                    break;
            }
        }
    }
}

int main(int argc, char** argv) {
    using namespace std::chrono;

    auto t1 = high_resolution_clock::now();
    auto t2 = t1;

    init();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Main loop
    State &state = State::getInstance();
    while(true){

        t2 = high_resolution_clock::now();
        duration<float> deltaT = t2 - t1;
        t1 = high_resolution_clock::now();
        for(int i = 0;  i < SimConst::UPDATE_ITERATIONS_EACH_FRAME; i++){
            state.update(SimConst::UPDATE_DELTA);
        }
        processInput(state);
        state.controlMinions(SimConst::UPDATE_ITERATIONS_EACH_FRAME * SimConst::UPDATE_DELTA);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        state.draw();

        SDL_GL_SwapWindow(Display::getInstance().window);

        if(state.getShouldEndProgram()){
            break;
        }
    }

    return 0;
}


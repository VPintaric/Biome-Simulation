#define GLM_FORCE_RADIANS // needed by GLM!

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <vector>
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/type_ptr.hpp>
#include <chrono>

#include "state/State.h"
#include "state/Log.h"
#include "constants/WindowConstants.h"
#include "state/Display.h"
#include "shader_handling/ProgramLoader.h"
#include "rendering/Renderer.h"
#include "constants/SimulationConstants.h"
#include "rendering/ColorModel.h"
#include "helpers/MinionModelCreator.h"

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

    std::shared_ptr<ProgramLoader> shaderProgram = std::make_shared<ProgramLoader>("resources/shaders/vertex_shaders/vertex_shader.glsl", "resources/shaders/fragment_shaders/fragment_shader.glsl");
    Renderer &renderer = Renderer::getInstance();
    renderer.addShaderProgram(WindowConst::DEFAULT_SHADER_NAME, shaderProgram);
    renderer.activateShaderProgram(WindowConst::DEFAULT_SHADER_NAME);

    MinionModelCreator::createMinionModel();
    
    renderer.setOrthoProjection(-WindowConst::WINDOW_WIDTH / 2., WindowConst::WINDOW_WIDTH / 2.,
                                        -WindowConst::WINDOW_HEIGHT / 2., WindowConst::WINDOW_HEIGHT / 2.);
    renderer.setCameraPosition(0, 0);
    renderer.identity();
    
    State::getInstance().spawnMinions(50);
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
        SDL_Event event{};
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_WINDOWEVENT){
                if(event.window.event == SDL_WINDOWEVENT_RESIZED){
                    Log().Get(logINFO) << "Window resized to " << event.window.data1 << "x" << event.window.data2;
                } else if(event.window.event == SDL_WINDOWEVENT_CLOSE){
                    Log().Get(logINFO) << "Window closed";
                    state.endProgram();
                    if(state.getShouldEndProgram()){
                        break;
                    }
                }
            }
        }

        t2 = high_resolution_clock::now();
        duration<float> deltaT = t2 - t1;
        t1 = high_resolution_clock::now();
        int nUpdates = ((int) (deltaT.count() / SimConst::UPDATE_DELTA) + 1);
        for(int i = 0;  i < nUpdates; i++){
            state.update(SimConst::UPDATE_DELTA);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        state.draw();
        
        SDL_GL_SwapWindow(Display::getInstance().window);
        
        if(state.getShouldEndProgram()){
            break;
        }
    }

    return 0;
}


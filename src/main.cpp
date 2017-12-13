#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/type_ptr.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>

#include "state/State.h"
#include "state/Log.h"
#include "constants/WindowConstants.h"
#include "state/Display.h"
#include "shader_handling/ProgramLoader.h"
#include "rendering/Renderer.h"
#include "constants/SimulationConstants.h"
#include "rendering/ColorModel.h"

std::string MINION2_NAME = "Minion 2";

void createBasicMinionModel(){
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

void createBasicMinionModelV2(){
    Log().Get(logDEBUG) << "Creating basic minion model (triangle)";
    
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    
    vertices.push_back((GLfloat) -1.0); vertices.push_back((GLfloat) 1.0);  vertices.push_back((GLfloat) 0.0);
    vertices.push_back((GLfloat) 1.0);  vertices.push_back((GLfloat) 1.0);  vertices.push_back((GLfloat) 0.0);
    vertices.push_back((GLfloat) 0.0);  vertices.push_back((GLfloat) -1.0); vertices.push_back((GLfloat) 0.0);

    colors.push_back((GLfloat) 1.0); colors.push_back((GLfloat) 0.2); colors.push_back((GLfloat) 0.0);
    colors.push_back((GLfloat) 1.0); colors.push_back((GLfloat) 0.2); colors.push_back((GLfloat) 0.0);
    colors.push_back((GLfloat) 1.0); colors.push_back((GLfloat) 0.2); colors.push_back((GLfloat) 0.0);
    
    std::shared_ptr<Model> m = std::make_shared<ColorModel>(vertices, colors);
    Renderer::getInstance().addNewModel(MINION2_NAME, m);
}

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

    createBasicMinionModel();
    createBasicMinionModelV2();
    
    renderer.setOrthoProjection(-WindowConst::WINDOW_WIDTH / 2.f, WindowConst::WINDOW_WIDTH / 2.f,
                                        -WindowConst::WINDOW_HEIGHT / 2.f, WindowConst::WINDOW_HEIGHT / 2.f);
    renderer.setCameraPosition(0, 0);
}

int main(int argc, char** argv) {
    init();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Main loop
    while(true){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_WINDOWEVENT){
                if(event.window.event == SDL_WINDOWEVENT_RESIZED){
                    Log().Get(logINFO) << "Window resized to " << event.window.data1 << "x" << event.window.data2;
                } else if(event.window.event == SDL_WINDOWEVENT_CLOSE){
                    Log().Get(logINFO) << "Window closed";
                    State::getInstance().endProgram();
                    if(State::getInstance().getShouldEndProgram()){
                        break;
                    }
                }
            }
        }

        Renderer &renderer = Renderer::getInstance();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        renderer.identity();
        renderer.scale(30, 30);
        renderer.getModel(SimConst::MINION_MODEL_NAME)->draw();
        
        SDL_GL_SwapWindow(Display::getInstance().window);
        
        if(State::getInstance().getShouldEndProgram()){
            break;
        }
    }

    return 0;
}


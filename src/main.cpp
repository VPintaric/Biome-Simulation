#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "state/State.h"
#include "state/Log.h"
#include "constants/WindowConstants.h"
#include "state/Display.h"
#include "shader_handling/ProgramLoader.h"
#include "rendering/Renderer.h"
#include "constants/SimulationConstants.h"
#include "rendering/ColorModel.h"

std::unique_ptr<ProgramLoader> shaderProgram;

void createBasicMinionModel(){
    Log().Get(logDEBUG) << "Creating basic minion model (triangle)";
    
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    
    vertices.push_back((GLfloat) -1.0); vertices.push_back((GLfloat) 1.0);
    vertices.push_back((GLfloat) 1.0);  vertices.push_back((GLfloat) 1.0);
    vertices.push_back((GLfloat) 0.0);  vertices.push_back((GLfloat) -1.0);

    colors.push_back((GLfloat) 0.0); colors.push_back((GLfloat) 0.2); colors.push_back((GLfloat) 0.0);
    colors.push_back((GLfloat) 0.0); colors.push_back((GLfloat) 0.2); colors.push_back((GLfloat) 0.0);
    colors.push_back((GLfloat) 0.75); colors.push_back((GLfloat) 0.2); colors.push_back((GLfloat) 0.0);
    
    std::shared_ptr<Model> m = std::make_shared<ColorModel>(vertices, colors);
    Renderer::getInstance().addNewModel(SimConst::MINION_MODEL_NAME, m);
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

    shaderProgram = std::make_unique<ProgramLoader>("resources/shaders/vertex_shaders/vertex_shader.glsl", "resources/shaders/fragment_shaders/fragment_shader.glsl");
    glUseProgram(shaderProgram->getId()); // using single shading program for now...

    createBasicMinionModel();
    
    // Following code is temporary

    GLfloat transMatrix[] = {10., 0., 0., 0., 10., 0., 0., 0., 1.};

    GLuint programId = shaderProgram->getId();
    glUniform1i(glGetUniformLocation(programId, "left"), -100);
    glUniform1i(glGetUniformLocation(programId, "width"), 200);
    glUniform1i(glGetUniformLocation(programId, "top"), -100);
    glUniform1i(glGetUniformLocation(programId, "height"), 200);
    glUniformMatrix3fv(glGetUniformLocation(programId, "transformMatrix"), 1, GL_FALSE, transMatrix);
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

        glClear(GL_COLOR_BUFFER_BIT);
        Renderer::getInstance().getModel(SimConst::MINION_MODEL_NAME)->draw();
        SDL_GL_SwapWindow(Display::getInstance().window);

        if(State::getInstance().getShouldEndProgram()){
            break;
        }
    }

    return 0;
}


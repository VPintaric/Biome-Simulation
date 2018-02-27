#include "state/Display.h"
#include "state/Log.h"

#include <string>

Display::Display() {

}

Display& Display::getInstance() {
    static Display instance;
    return instance;
}

SDL_Window* Display::createWindow(const std::string title, int x, int y, int w, int h, Uint32 flags) {
    if(window != nullptr){
        Log().Get(logWARNING) << "SDL window instance already exists, ignoring window creation call...";
        return window;
    }
    
    Log().Get(logDEBUG) << "Creating SDL window with parameters: title = " << title
            << ", x = " << x << ", y = " << y << ", w = " << w << ", h = " << h <<
            ", flags = " << flags;
    
    window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
    if(window == nullptr){
        Log().Get(logERROR) << "Failed to create SDL window";
    }
    
    return window;
}

SDL_GLContext Display::attachContext() {
    Log().Get(logDEBUG) << "Attaching GL context to window...";
    
    if(window == nullptr){
        Log().Get(logWARNING) << "No window instantiated to attach GL context to...";
        return nullptr;
    }
    glContext = SDL_GL_CreateContext(window);
    return glContext;
}


Display::~Display() {
    if(window != nullptr){
        Log().Get(logDEBUG) << "Destroying SDL window...";
        SDL_DestroyWindow(window);
    }
    
    if(glContext != nullptr){
        Log().Get(logDEBUG) << "Destroying GL context...";
        SDL_GL_DeleteContext(glContext);
    }
}


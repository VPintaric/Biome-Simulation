#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL_video.h>
#include <string>


class Display {
public:
    virtual ~Display();
    static Display& getInstance();
    SDL_Window* createWindow(const std::string title, int x, int y, int w, int h, Uint32 flags);
    SDL_GLContext attachContext();
    
    SDL_Window* window;
    SDL_GLContext glContext;
private:
    Display();
};

#endif /* DISPLAY_H */


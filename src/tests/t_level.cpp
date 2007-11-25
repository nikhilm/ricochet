#include<iostream>

#include<SDL/SDL.h>

#include "resource.h"
#include "level.h"
#include "levelparser.h"

int main() {
    SDL_Surface *screen;
    bool loopRunning = true;

    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout<<"Can't initialize SDL"<<SDL_GetError()<<std::endl;
        exit(1);
    }
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);

    if (screen == NULL) {
        std::cout<<"Unable to set video mode: "<<SDL_GetError()<<std::endl;
        exit(1);
    }

    SDL_WM_SetCaption("Blocks Test", NULL);

    rtResource::init();
    rtLevelParser::init();
    rtPhoton p(rtBlock::DOWN, 325, 10);
    
    rtLevel *lvl = rtLevelParser::getLevelFromPasscode(std::string("bicycle"));
    
    SDL_Event event;
    while(loopRunning)
    {
        while(SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type) loopRunning = false;
            if (SDL_KEYDOWN == event.type && SDLK_ESCAPE == event.key.keysym.sym) loopRunning = false;
            lvl->handleEvent(event);
        }
        SDL_FillRect(screen, NULL, 0x0);

        lvl->display(screen, 0, 0);
        lvl->update();
        SDL_Flip(screen);
    }
    rtResource::cleanup();
}

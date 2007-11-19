#include<iostream>

#include<SDL/SDL.h>

#include "resource.h"
#include "textutil.h"

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
    atexit(rtResource::cleanup);
    
    if(!rtTextUtil::init()) {
        std::cout<<"Error loading font rendering system. Check that SDL_ttf is installed and fonts are in the right place\n";
        exit(1);
    }
    atexit(rtTextUtil::cleanup);

    SDL_Event event;
    while(loopRunning)
    {
        while(SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type) loopRunning = false;
            if (SDL_KEYDOWN == event.type && SDLK_ESCAPE == event.key.keysym.sym) loopRunning = false;
        }
        SDL_Color c = {255, 0, 0};
        SDL_Rect pos;
        pos.x = 200;
        pos.y = 20;
        SDL_BlitSurface(rtTextUtil::render("hello world!", c, RT_LARGE_FONT), NULL, screen, &pos);
        
        rtTextUtil::renderOn("hello world!", c, RT_LARGE_FONT, screen, 400, 400);

        SDL_Flip(screen);
    }
}

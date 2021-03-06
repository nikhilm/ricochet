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

    SDL_Color c = {255, 0, 0};
    rtTextUtil::render("hello\nworld!", c, rtTextUtil::RT_LARGE_FONT, screen, 400, 400);
    rtTextUtil::render("These are some instructions on the\navailabitlit\n of disgutsting'n\ncurnchies", c, rtTextUtil::RT_SMALL_FONT, screen, 220, 20, rtTextUtil::ALIGN_RIGHT);
    SDL_Event event;
    while(loopRunning)
    {
        while(SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type) loopRunning = false;
            if (SDL_KEYDOWN == event.type && SDLK_ESCAPE == event.key.keysym.sym) loopRunning = false;
        }

        SDL_Flip(screen);
    }
}

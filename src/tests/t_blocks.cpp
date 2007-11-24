#include<iostream>

#include<SDL/SDL.h>

#include "resource.h"
#include "blocks.h"

int main() {
    SDL_Surface *screen;
    bool loopRunning = true;

    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout<<"Can't initialize SDL"<<SDL_GetError()<<std::endl;
        exit(1);
    }
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(600, 500, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);

    if (screen == NULL) {
        std::cout<<"Unable to set video mode: "<<SDL_GetError()<<std::endl;
        exit(1);
    }

    SDL_WM_SetCaption("Blocks Test", NULL);

    rtResource::init();
    rtPhoton p(rtBlock::DOWN, 100, 10);
    
    rtBlock *blocks[9] = {new rtLauncher(rtBlock::UP, 9*rtBlock::WIDTH, 9*rtBlock::HEIGHT),
                          new rtSwitch(rtBlock::UP, 5*rtBlock::WIDTH, 5*rtBlock::HEIGHT),
                          new rtBomb(rtBlock::LEFT, 3*rtBlock::WIDTH, 4*rtBlock::HEIGHT),
                          new rtPrism(rtBlock::RIGHT, 2*rtBlock::WIDTH, 5*rtBlock::HEIGHT),
                          new rtArrow(rtBlock::LEFT, 6*rtBlock::WIDTH, 5*rtBlock::HEIGHT),
                          new rtDeflector(rtBlock::LEFT, 6*rtBlock::WIDTH, 7*rtBlock::HEIGHT),
                          new rtPrism(rtBlock::UP, 2*rtBlock::WIDTH, 7*rtBlock::HEIGHT),
                          new rtArrow(rtBlock::LEFT, 5*rtBlock::WIDTH, 9*rtBlock::HEIGHT),
                          new rtEnergiser(rtBlock::UP, 6*rtBlock::WIDTH, 1*rtBlock::HEIGHT)};

    SDL_Event event;
    while(loopRunning)
    {
        while(SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type) loopRunning = false;
            if (SDL_KEYDOWN == event.type && SDLK_ESCAPE == event.key.keysym.sym) loopRunning = false;
            
        }
        SDL_FillRect(screen, NULL, 0x0);

        for(int i = 0; i < 9; ++i) {
            if(p.x() == blocks[i]->x()+rtBlock::WIDTH/2 && p.y() == blocks[i]->y()+rtBlock::HEIGHT/2) {
                std::cout<<"Photon handled\n";
                blocks[i]->handlePhoton(p);
            }
            blocks[i]->display(screen, 0, 0);
        }
        p.move();
        p.display(screen, 0, 0);
        SDL_Flip(screen);
        SDL_Delay(5);
    }
    rtResource::cleanup();
}

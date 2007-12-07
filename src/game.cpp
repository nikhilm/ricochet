#include <iostream>

#include <SDL/SDL.h>

#include "resource.h"
#include "textutil.h"
#include "state.h"
#include "levelparser.h"
#include "game.h"

rtState * rtGame::currentState = NULL;
SDL_Surface * rtGame::screen = NULL;

bool rtGame::gameRunning = true;
bool rtGame::stateChange = false;
    
void rtGame::run() {
    //initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout<<"Can't initialize SDL"<<SDL_GetError()<<std::endl;
        exit(1);
    }
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);

    if (screen == NULL) {
        std::cout<<"Unable to set video mode: "<<SDL_GetError()<<std::endl;
        exit(1);
    }

    SDL_WM_SetCaption("Ricochet", NULL);
    SDL_EnableUNICODE(SDL_DISABLE);
    
    //initialize resources, levels and text
    if(!rtResource::init()) {
        std::cerr<<"Could not load required game resources.\n";
        exit(1);
    }
    atexit(rtResource::cleanup);

    if(!rtLevelParser::init()) {
        std::cerr<<"Could not load levels.\n";
        exit(0);
    }
    
    if(!rtTextUtil::init()) {
        std::cerr<<"Error loading font rendering system. Check that SDL_ttf is installed and fonts are in the right place\n";
        exit(1);
    }
    atexit(rtTextUtil::cleanup);

    //begin game
    gameRunning = true;
    
    currentState = new rtTransitionState(0);//rtStartState;
    currentState->firstDisplay(screen);
    
    SDL_Event event;
    while(gameRunning)
    {
        if(stateChange) {
            rtState * tmp = currentState->nextState;
            if(tmp) {
                delete currentState;
                currentState = NULL;
                currentState = tmp;
                
                SDL_FillRect(screen, NULL, 0x0);
                currentState->firstDisplay(screen);
            }
            stateChange = false;
        }
        
        while(SDL_PollEvent(&event))
        {
            if(currentState->handleEvent(event))
                ;
            else if (SDL_QUIT == event.type) gameRunning = false;
            else if (SDL_KEYDOWN == event.type && SDLK_ESCAPE == event.key.keysym.sym) gameRunning = false;
        }
        currentState->update();
        currentState->display(screen);
        
        SDL_Flip(screen);
    }
}

// void rtGame::changeState(rtState * s) {
//     std::cout<<"Changing state\n\n";
//     if(currentState != NULL)
//         delete currentState;
//     currentState = NULL;
//     currentState = s;
//     
//     SDL_FillRect(screen, NULL, 0x0);
//     currentState->firstDisplay(screen);
// }

void rtGame::changeState() {
    stateChange= true;
    
}

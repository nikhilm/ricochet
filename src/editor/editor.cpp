/*
 * Copyright (C)      2007  Nikhil Marathe <nsm.nikhil@gmail.com>
 *               
 * This file is part of the Ricochet project.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef RT_EDITOR_H
#define RT_EDITOR_H

#include <iostream>
#include <string>
#include <vector>

#include <SDL/SDL.h>

#include "resource.h"
#include "level.h"
#include "blocks.h"

struct rtBlkWrp {
    rtBlock * block;
    char type;
    bool userControlled;
};

class rtEditor {
    rtBlkWrp m_grid[rtLevel::GRID_WIDTH][rtLevel::GRID_HEIGHT];
    
    rtFakeLevel m_levelData;
    
    const char * m_fileName;
    
    int getGridCoordX() const {
        int x;
        SDL_GetMouseState(&x, NULL);
        return x / rtBlock::WIDTH;
    }
    
    int getGridCoordY() const {
        int y;
        SDL_GetMouseState(NULL, &y);
        return y / rtBlock::HEIGHT;
    }
    
    rtBlkWrp & currentBlkWrp() {
        return m_grid[getGridCoordX()][getGridCoordY()];
    }
    
    void setBlockDirection(int direction) {
        rtBlkWrp bw = currentBlkWrp();
        if(bw == NULL) return;
        bw.block->setDirection(direction);
    }
    
    void setBlock(char type) {
        rtBlkWrp bw;
        bw.type = type;
        bw.userControlled = true;
        
        switch(type) {
            case 'a':
                bw.block = new rtArrow(rtBlock::UP, 0, 0);
                bw.userControlled = true;
                break;                
                
            case 'b':
                bw.block = new rtBomb(rtBlock::UP, 0, 0);
                break;                
                
            case 'd':
                bw.block = new rtDeflector(rtBlock::UP, 0, 0);
                bw.userControlled = true;
                break;                
                
            case 'l':
                bw.block = new rtLauncher(rtBlock::UP, 0, 0);
                break;                
                
            case 'p':
                bw.block = new rtPrism(rtBlock::UP, 0, 0);
                bw.userControlled = true;
                break;                
                
            case 's':
                bw.block = new rtSwitch(rtBlock::UP, 0, 0);
                break;                
                
            case 'w':
                bw.block = new rtWall(rtBlock::UP, 0, 0);
                break;
                
                
            case 'x':
                bw.block = new rtEnergiser(rtBlock::UP, 0, 0);
                break;
        }
        
        int x = getGridCoordX(), y = getGridCoordY();
        bw.block->setX(x);
        bw.block->setY(y);
        m_grid[x][y] = bw;
        
    }
    
    void deleteBlock() {
        m_grid[getGridCoordX()][getGridCoordY()] = NULL;
    }
    
    void handleEvent(SDL_Event evt) {
        /* Left Ctrl+s -> Save level
         * Delete -> delete current block
         * Arrow keys -> Set direction of current block
         * Any valid letter -> create block of that type
         */
        
        if(!evt.type == SDL_KEYDOWN) return;
        
        if(evt.keysym.mod == KMOD_LCTRL && evt.keysym.sym == SDLK_s) {
            saveLevel();
        }
        
        else if(evt.keysym.sym == SDLK_DELETE) {
            deleteBlock();
        }
        
        else {
            switch(evt.keysym.sym) {
                case SDLK_UP:
                    setBlockDirection(rtBlock::UP);
                    break;
                case SDLK_DOWN:
                    setBlockDirection(rtBlock::DOWN);
                    break;
                case SDLK_RIGHT:
                    setBlockDirection(rtBlock::RIGHT);
                    break;
                case SDLK_LEFT:
                    setBlockDirection(rtBlock::LEFT);
                    break;
                    
                
                // copied from rtLevelParser::getBlock and modified
                case SDLK_a:
                    setBlock('a');
                    break;
                
                case SDLK_b:
                    setBlock('b');
                    break;
                
                case SDLK_d:
                    setBlock('d');
                    break;
                
                case SDLK_l:
                    setBlock('l');
                    break;
                
                case SDLK_p:
                    setBlock('p');
                    break;
                
                case SDLK_s:
                    setBlock('s');
                    break;
                
                case SDLK_w:
                    setBlock('w');
                    break;
                
                case SDLK_x:
                    setBlock('x');
                    break;
            }
        }
    }
public:
    rtEditor(const char * fN, const char * t, char * st, char * code) {
        m_fileName = fN;
        m_levelData.title = t;
        m_levelData.subtitle = st;
        m_levelData.passcode = code;
        m_levelData.userListStr = "";
        m_levelData.gridStr = "";
        
        for(int i = 0; i < rtLevel::GRID_WIDTH; ++i) {
            for(int j = 0; j < rtLevel::GRID_HEIGHT; ++j) {
                m_grid[i][j] = NULL;
            }
        }
    }
    
    void run() {
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
        rtLevelParser::init();
        rtPhoton p(rtBlock::DOWN, 325, 10);
    
        rtLevel *lvl = rtLevelParser::getLevel(1);
    
        SDL_Event event;
        while(loopRunning)
        {
            while(SDL_PollEvent(&event))
            {
                if (SDL_QUIT == event.type) loopRunning = false;
                else if (SDL_KEYDOWN == event.type && SDLK_ESCAPE == event.key.keysym.sym) loopRunning = false;
                
                else handleEvent(event);
            
            }
            SDL_FillRect(screen, NULL, 0x0);

            displayGrid(screen);
            SDL_Flip(screen);
        }
        rtResource::cleanup();
    }
};

int main(int argc, char ** argv) {
    if(argc != 5) {
        std::cout<<"Usage: "<<argv[0]<<" <file> <title> <subtitle> <passcode>\n";
        return 0;
    }
    
    rtEditor(argv[1], argv[2], argv[3], argv[4]);
    rtEditor.run();
    return 0;
}

#endif

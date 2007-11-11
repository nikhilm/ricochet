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
#include <fstream>
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
        
    void setBlockDirection(int direction) {
        int x = getGridCoordX(), y = getGridCoordY();
        if(m_grid[x][y].block == NULL) return;
        std::cout<<"Not null\n";
        m_grid[x][y].block->setDirection(direction);
        std::cout<<"Direction is "<<m_grid[x][y].block->directionToString()<<std::endl;
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
        int x = getGridCoordX(), y = getGridCoordY();
        m_grid[x][y].block = NULL;
        m_grid[x][y].type = 'e';
        m_grid[x][y].userControlled = false;
    }
    
    void saveLevel() {
        std::ofstream out(m_fileName, std::ios::app);
        
        out<<"\n> "<<m_levelData.title<<"\n< "<<m_levelData.subtitle<<"\n& "<<m_levelData.passcode<<"\n@ ";
        
        for(int i = 0; i < rtLevel::GRID_WIDTH; ++i) {
            for(int j = 0; j < rtLevel::GRID_HEIGHT; ++j) {
                if(m_grid[i][j].userControlled) {                    
                    m_levelData.gridStr += "eU";
                    m_levelData.userListStr += m_grid[i][j].type;
                    m_levelData.userListStr += m_grid[i][j].block->directionToString();
                }
                else {
                    m_levelData.gridStr += m_grid[i][j].type;
                    if(m_grid[i][j].block == NULL)
                        m_levelData.gridStr += "U";
                    else
                        m_levelData.gridStr += m_grid[i][j].block->directionToString();
                    
                }
            }
            m_levelData.gridStr += "\n";
        }
        
        out<<m_levelData.userListStr<<std::endl;
        out<<m_levelData.gridStr<<"\n!\n";
        out.close();
    }
    
    void displayGrid(SDL_Surface * surf) {
        for(int i = 0; i < rtLevel::GRID_WIDTH; ++i) {
            for(int j = 0; j < rtLevel::GRID_HEIGHT; ++j) {
                if(m_grid[i][j].block != NULL)
                    m_grid[i][j].block->display(surf, 0, 0);
            }
        }
    }
    
    void handleEvent(SDL_Event evt) {
        /* Left Ctrl+s -> Save level
         * Delete -> delete current block
         * Arrow keys -> Set direction of current block
         * Any valid letter -> create block of that type
         */
        
        if(evt.type != SDL_KEYDOWN) return;
        
        if(evt.key.keysym.mod == KMOD_LCTRL && evt.key.keysym.sym == SDLK_s) {
            saveLevel();
        }
        
        else if(evt.key.keysym.sym == SDLK_DELETE) {
            deleteBlock();
        }
        
        else {
            std::cout<<"Else block\n";
            switch(evt.key.keysym.sym) {
                case SDLK_UP:
                    std::cout<<"Setting direc\n";
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
                m_grid[i][j].block = NULL;
                m_grid[i][j].type = 'e';
                m_grid[i][j].userControlled = false;
                
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

        screen = SDL_SetVideoMode(rtBlock::WIDTH * rtLevel::GRID_WIDTH, rtBlock::HEIGHT * rtLevel::GRID_HEIGHT, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);

        if (screen == NULL) {
            std::cout<<"Unable to set video mode: "<<SDL_GetError()<<std::endl;
            exit(1);
        }

        SDL_WM_SetCaption("Level Editor", NULL);

        rtResource::init();
        
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
    
    rtEditor(argv[1], argv[2], argv[3], argv[4]).run();
    return 0;
}

#endif

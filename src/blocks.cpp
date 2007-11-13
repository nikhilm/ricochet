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

#include "blocks.h"

rtBlock * getBlock(int type, char d, int x, int y) {
    int dir = intDirection(d);    
    
    std::cout<<"Recd x "<<x<<" y "<<y<<std::endl;
    switch(type) {
        case rtBlock::ARROW:
            return(new rtArrow(dir, x, y));
            
            
        case rtBlock::BOMB:
            return(new rtBomb(dir, x, y));
            
            
        case rtBlock::DEFLECTOR:
            return(new rtDeflector(dir, x, y));
            
            
        case rtBlock::LAUNCHER:
            return(new rtLauncher(dir, x, y));
            
            
        case rtBlock::PRISM:
            return(new rtPrism(dir, x, y));
            
            
        case rtBlock::SWITCH:
            return(new rtSwitch(dir, x, y));
            
            
        case rtBlock::WALL:
            return(new rtWall(dir, x, y));
            
            
        case rtBlock::ENERGISER:
            return(new rtEnergiser(dir, x, y));
            
        default:
            return NULL;    
    }
}

char charDirection(int dir) {
    switch(dir) {
        case rtBlock::UP: return 'U';
        case rtBlock::DOWN: return 'D';
        case rtBlock::LEFT: return 'L';
        case rtBlock::RIGHT: return 'R';
        default: return 'U';
    }
}

int intDirection(char dir) {
    switch(dir) {
        case 'U':return rtBlock::UP;
        case 'D':return rtBlock::DOWN;
        case 'L':return rtBlock::LEFT;
        case 'R':return rtBlock::RIGHT;
        default: return rtBlock::UP;
    }
}

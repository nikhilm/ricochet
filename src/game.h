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

#ifndef RT_GAME_H
#define RT_GAME_H

#include <iostream>

#include <SDL/SDL.h>

class rtState;

class rtGame {
    static rtState * currentState;
    static rtState * nextState;
    
    static SDL_Surface * screen;
    
    static bool gameRunning;
    static bool stateChanged;
    
public:
    
    enum { SCREEN_WIDTH=800, SCREEN_HEIGHT=600 };    
    
    static SDL_Color RT_TEXT_COLOR;
    
    static void run();
    
    static void changeState(rtState * s);
};

#endif

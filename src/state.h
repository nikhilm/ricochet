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

#ifndef RT_STATE_H
#define RT_STATE_H

#include <iostream>
#include <string>

#include <SDL/SDL.h>

#include "resource.h"
#include "textutil.h"

class rtGame;

class rtState {
protected:
    rtGame * game;
public:
    rtState(rtGame *g) { game = g; }
    
    virtual void firstDisplay(SDL_Surface *) {}
    
    virtual void display(SDL_Surface *) {}
    
    virtual void update() {}
    
    virtual bool handleEvent(SDL_Event &) {};
};

class rtStartState : public rtState {
public:
    rtStartState(rtGame * game) : rtState(game) {}
    void firstDisplay(SDL_Surface *);
    bool handleEvent(SDL_Event &);
};

/* A general state to display text.
 * Subclasses should just provide their own text */
class rtPausedState : public rtState {    
    SDL_Color color;
protected:    
    std::string largeText; //title and stuff
    std::string smallText;
    
public:
    
    rtPausedState(rtGame * game) : rtState(game) {
        largeText = smallText = "";
        
        color.r = 255;
        color.g = 0;
        color.b = 0;
    }
    
    void firstDisplay(SDL_Surface *);
};

class rtGameOver : public rtPausedState {
public:
    rtGameOver(rtGame * game, bool success);
};

#endif

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
#include <SDL/SDL_ttf.h>

#include "resource.h"
#include "textutil.h"

class rtGame;

class rtState {
protected:
    rtGame * game;
public:
    rtState(rtGame *g) { game = g; }
    
    virtual void firstDisplay(SDL_Surface *surf) {}
    
    virtual void display(SDL_Surface *) {}
    
    virtual void update() {}
    
    virtual bool handleEvent(SDL_Event &) {};
};

class rtStartState : public rtState {
public:
    rtStartState(rtGame * g) : rtState(g) {}
    void firstDisplay(SDL_Surface * surf) {
        SDL_BlitSurface(rtResource::loadImage("intro", "bg"), NULL, surf, NULL);

    }
};

class rtPaused : public rtState {
protected:
    std::string title, text;
public:
    rtPaused(rtGame * g) : rtState(g) {
        title = text = "";
    }
    
    void firstDisplay(SDL_Surface * surf) {
        SDL_Color c = {255, 0, 0};
        rtTextUtil::render(title.c_str(), c, RT_LARGE_FONT, surf, 400, 50, rtTextUtil::ALIGN_CENTER);
        rtTextUtil::render(text.c_str(), c, RT_SMALL_FONT, surf, 400, 300, rtTextUtil::ALIGN_CENTER);
    }
};

class rtGameOver : public rtPaused {
public:
    rtGameOver(rtGame * g, bool success) : rtPaused(g) {
        if(success) {
            title = "Superb!";
            text = "You've won the game";
        }
        else {
            title = "Oops!";
            text = "You've lost. Better luck next time.";
        }
    }
};
#endif

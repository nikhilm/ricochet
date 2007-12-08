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
#include <ctype.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "resource.h"
#include "textutil.h"
#include "menu.h"

class rtStartState;

class rtState {
public:    
    rtState * nextState;
    virtual void firstDisplay(SDL_Surface *surf) {}
    
    virtual void display(SDL_Surface *) {}
    
    virtual void update() {}
    
    virtual bool handleEvent(SDL_Event &) {};
};

/*******************************
 * Start state related actions *
 *******************************/

class rtNewGameAction : public rtMenuAction {
    rtStartState * st;
public:
    rtNewGameAction(rtStartState *);
    void trigger(const SDL_Event& evt);
};

class rtPasscodeAction : public rtMenuAction {
    rtStartState * st;
public:
    rtPasscodeAction(rtStartState *);
    void trigger(const SDL_Event& evt);
};

class rtQuitAction : public rtMenuAction {
public:
    rtQuitAction(rtStartState * state) {};
    void trigger(const SDL_Event& evt);
};

/***************
 * Start state *
 ***************/
 
class rtStartState : public rtState {
    rtMenu * menu;
public:
    rtStartState() {
        menu = new rtMenu(250, 250);
        menu->addItem("New Game", new rtNewGameAction(this));
        menu->addItem("Enter Passcode", new rtPasscodeAction(this));
        menu->addItem("Quit", new rtQuitAction(this));
    }
    
    void firstDisplay(SDL_Surface * surf) {
        display(surf);
    }
    
    void display(SDL_Surface * surf) {        
        SDL_BlitSurface(rtResource::loadImage("intro", "bg"), NULL, surf, NULL);
        menu->display(surf);
    }
    
    bool handleEvent(SDL_Event &evt) {
        menu->handle(evt);
    }
};

/*
 * Simple paused state which displays text and moves to nextState when clicked
 */
class rtPaused : public rtState {
protected:
    std::string title, text;
    bool change;
public:
    rtPaused() {
        title = text = "";
        nextState = NULL;
        change = false;
    }
    
    void firstDisplay(SDL_Surface * surf) {
        SDL_Color c = {255, 0, 0};
        rtTextUtil::render(title.c_str(), c, rtTextUtil::RT_LARGE_FONT, surf, 400, 60, rtTextUtil::ALIGN_CENTER);
        rtTextUtil::render(text.c_str(), c, rtTextUtil::RT_SMALL_FONT, surf, 400, 310, rtTextUtil::ALIGN_CENTER);
        
    }

    bool handleEvent(SDL_Event &);
};

class rtGameOver : public rtPaused {
public:
    rtGameOver(bool success) {
        if(success) {
            title = "Superb!";
            text = "You've won the game";
        }
        else {
            title = "Oops!";
            text = "You've lost. Better luck next time.";
        }
        nextState = new rtStartState;
    }
    
};

/****************************
 * Load level from passcode *
 ***************************/
class rtPasscodeState : public rtPaused {
    std::string code;
public:
    rtPasscodeState();
    
    bool handleEvent(SDL_Event &evt);
    
    void display(SDL_Surface *surf);
};

/****************************************
 * Transition from one level to another *
 ***************************************/
class rtTransitionState : public rtPaused {
public:
    rtTransitionState(int);
};
        
#endif

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

//TODO: Move this to global settings

class rtState {
public:    
    
    virtual void firstDisplay(SDL_Surface *surf) {}
    
    virtual void display(SDL_Surface *) {}
    
    virtual void update() {}
    
    virtual bool handleEvent(SDL_Event &) {};
};

/*******************************
 * Start state related actions *
 *******************************/

// class rtNewGameAction : public rtMenuAction {
// public:
//     void trigger(const SDL_Event& evt) {
//         std::cout<<"Starting new game\n";
//     }
// };
// 
// class rtPasscodeAction : public rtMenuAction {
// public:
//     void trigger(const SDL_Event& evt) {
//         std::cout<<"Going to passcode\n";
//     }
// };
// 
// class rtQuitAction : public rtMenuAction {
// public:
//     void trigger(const SDL_Event& evt) {
//         exit(0);
//     }
// };

// /***************
//  * Start state *
//  ***************/
//  
// class rtStartState : public rtState {
//     rtMenu * menu;
// public:
//     rtStartState() {
//         menu = new rtMenu(250, 250);
//         menu->addItem("New Game", new rtNewGameAction);
//         menu->addItem("Enter Passcode", new rtPasscodeAction);
//         menu->addItem("Quit", new rtQuitAction);
//     }
//     
//     void firstDisplay(SDL_Surface * surf) {
//         display(surf);
//     }
//     
//     void display(SDL_Surface * surf) {        
//         SDL_BlitSurface(rtResource::loadImage("intro", "bg"), NULL, surf, NULL);
//         menu->display(surf);
//     }
//     
//     bool handleEvent(SDL_Event &evt) {
//         menu->handle(evt);
//     }
// };
// 
// /*
//  * Simple paused state which displays text and moves to nextState when clicked
//  */
// class rtPaused : public rtState {
// protected:
//     std::string title, text;
//     rtState * nextState;
//     bool change;
// public:
//     rtPaused(/*rtGame * g) : rtState(g*/) {
//         title = text = "";
//         nextState = NULL;
//         change = false;
//     }
//     
//     void firstDisplay(SDL_Surface * surf) {
//         rtTextUtil::render(title.c_str(), RT_TEXT_COLOR, RT_LARGE_FONT, surf, 400, 50, rtTextUtil::ALIGN_CENTER);
//         rtTextUtil::render(text.c_str(), RT_TEXT_COLOR, RT_SMALL_FONT, surf, 400, 300, rtTextUtil::ALIGN_CENTER);
//     }
//     
//     bool handleEvent(SDL_Event &evt) {
//         if(evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT) {
//             rtGame::changeState(nextState);
//             return true;
//         }
//         return false;
//     }
//     
// };
// 
// class rtGameOver : public rtPaused {
// public:
//     rtGameOver(/*rtGame * g, */bool success)/* : rtPaused(g)*/ {
//         if(success) {
//             title = "Superb!";
//             text = "You've won the game";
//         }
//         else {
//             title = "Oops!";
//             text = "You've lost. Better luck next time.";
//         }
//         nextState = new rtStartState;
//     }
//     
// };
// 
// /****************************
//  * Load level from passcode *
//  ***************************/
// class rtPasscodeState : public rtPaused {
//     std::string code;
// public:
//     rtPasscodeState() {
//         code = "";
//         title = "Enter Passcode";
//         text = "_";
//     }
//     
//     bool handleEvent(SDL_Event &evt) {
//         if(evt.type == SDL_KEYDOWN) {
//             if(isalnum(evt.key.keysym.sym)) {
//                 code += evt.key.keysym.sym;
//                 return true;
//             }
//             
//             else if(evt.key.keysym.sym == SDLK_BACKSPACE && !code.empty()) {
//                 code = code.substr(0, code.length() - 1);
//                 return true;
//             }
//             
//             else if(evt.key.keysym.sym == SDLK_RETURN) {
//                 rtLevelParser::getLevelFromPasscode(code);
//                 std::cout<<"Getting level from passcode\n";
//                 return true;
//             }
//         }
//         return rtPaused::handleEvent(evt);
//     }
//     
//     void display(SDL_Surface *surf) {
//         text = code + '_';
//         SDL_FillRect(surf, NULL, 0x000000);
//         firstDisplay(surf);
//     }
// };

#endif

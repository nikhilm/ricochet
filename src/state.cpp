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

#include "state.h"
#include "levelparser.h"
#include "game.h"

void rtNewGameAction::trigger(const SDL_Event& evt) {
    rtGame::changeState(rtLevelParser::getLevel(0));
}

void rtPasscodeAction::trigger(const SDL_Event& evt) {
    rtGame::changeState(new rtPasscodeState);
}

void rtQuitAction::trigger(const SDL_Event& evt) {
    exit(0);
}

bool rtPaused::handleEvent(SDL_Event &evt) {
    if(evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT) {
        rtGame::changeState(nextState);
        return true;
    }
    return false;
}
/******************
 * Passcode state *
 *****************/
rtPasscodeState::rtPasscodeState() {
    code = "";
    title = "Enter Passcode";
    text = "_";
}

bool rtPasscodeState::handleEvent(SDL_Event &evt) {
    if(evt.type == SDL_KEYDOWN) {
        if(isalnum(evt.key.keysym.sym)) {
            code += evt.key.keysym.sym;
            return true;
        }
    
        else if(evt.key.keysym.sym == SDLK_BACKSPACE && !code.empty()) {
            code = code.substr(0, code.length() - 1);
            return true;
        }
    
        else if(evt.key.keysym.sym == SDLK_RETURN) {
            rtLevelParser::getLevelFromPasscode(code);
            std::cout<<"Getting level from passcode\n";
            return true;
        }
    }
    return rtPaused::handleEvent(evt);
}

void rtPasscodeState::display(SDL_Surface *surf) {
    text = code + '_';
    SDL_FillRect(surf, NULL, 0x000000);
    firstDisplay(surf);
}

/**************
 * Transition *
 *************/
rtTransitionState::rtTransitionState(int levelDone) {
    if(levelDone + 1 >= rtLevelParser::levelCount()) {
        title = "Congratulations!";
        text = "You've finished Ricochet\n";
        nextState = new rtStartState;
    }
    else {
        nextState = rtLevelParser::getLevel(levelDone + 1);
        rtLevel * lvl = (rtLevel *)nextState;
        
        title = "Next level\n" + lvl->m_title;
        
        text = lvl->m_subtitle;
        text += "\n\nPasscode: " + lvl->m_passcode;
    }
}
        
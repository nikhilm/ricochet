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
#include "game.h"

/***************
 * Start state *
 **************/

void rtStartState::firstDisplay(SDL_Surface * surf) {
    //SDL_BlitSurface(rtResource::loadImage("intro", "bg"), NULL, surf, NULL);
}

bool rtStartState::handleEvent(SDL_Event &evt) {
    if(evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT) {
        //game->changeState(new rtGameOver(game, false));
        return true;
    }
    return false;
}

/*************** **
 * Paused states *
 ****************/
/* A general state to display text.
 * Subclasses should just provide their own text */
void rtPausedState::firstDisplay(SDL_Surface * surf) {
    SDL_FillRect(surf, NULL, 0x000000);
    std::cout<<largeText<<std::endl;
    rtTextUtil::render(largeText.c_str(), color, RT_LARGE_FONT, surf, 400, 100, rtTextUtil::ALIGN_CENTER);
    rtTextUtil::render(smallText.c_str(), color, RT_SMALL_FONT, surf, 50, 200);
}


rtGameOver::rtGameOver(rtGame * game, bool success) : rtPausedState(game) {
    largeText = (success ? "Amazing!" : "Oh no!");
    smallText = (success ? "You've won the game!" : "Sorry you lost! Try again");
}

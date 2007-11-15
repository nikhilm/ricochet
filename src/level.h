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

#ifndef RT_LEVEL_H
#define RT_LEVEL_H

#include <iostream>
#include <string>
#include <vector>

#include <SDL/SDL.h>

class rtBlock;
class rtPhoton;
class rtSwitch;

/*
 * In init levels are parsed and stored as a quick struct.
 * Its only when the level is requested in makeLevel that
 * the actual objects are generated.
 */
 
struct rtFakeLevel {
    std::string title, subtitle, passcode, userListStr, gridStr;
};


class rtLevel {
    public:
    std::string m_title, m_subtitle, m_passcode;
    
    std::vector<rtBlock *> m_userBlockList;
    
    std::vector<rtBlock *> m_grid;
    
    rtPhoton * m_photon;
    
    int m_switchesAlive;
    
    rtBlock * m_activeBlock; //block which has the mouse over it
    
    bool m_dragInProgress, m_clicked;

public:
    enum {GRID_WIDTH = 12,
        GRID_HEIGHT = 12,
        DOCK_OFFSET_X_LOGICAL=GRID_WIDTH,
        DOCK_OFFSET_Y=0,
        DOCK_PADDING=10,
        DOCK_COLS=2
    };
    
    const int DOCK_OFFSET_X;
    
    rtLevel();
    
    void setTitle(std::string title);
    
    void setSubtitle(std::string stitle);
    
    void setPasscode(std::string code);
    
    void addUserBlock(rtBlock * b);
    
    void addGridBlock(rtBlock * b);
    
    void display(SDL_Surface *surf, int offsetX, int offsetY);
    
    void registerPhoton(rtPhoton *);
    
    void update();
    
    bool handleEvent(SDL_Event);
    
    void switchToggled(rtSwitch *);
    
    void gameOver(bool);
    
    rtBlock * getBlockAt(int, int);
};

#endif

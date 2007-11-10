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

#include "blocks.h"

class rtLevel {
    public:
    std::string m_title, m_subtitle, m_passcode;
    
    std::vector<rtBlock *> m_userBlockList;
    
    std::vector<rtBlock *> m_grid;
     
    rtBlock * m_b;
    

public:
    enum {GRID_WIDTH = 12, GRID_HEIGHT = 12};
    
    rtLevel() {
        m_title = m_subtitle = m_passcode = "";
        m_userBlockList = std::vector<rtBlock *>();
        m_grid = std::vector<rtBlock *>();
    }
    
    void setTitle(std::string title) {
        m_title = title;
    }
    
    void setSubtitle(std::string stitle) {
        m_subtitle = stitle;
    }
    
    void setPasscode(std::string code) {
        m_passcode = code;
    }
    
    void addUserBlock(rtBlock * b) {
        if(b == NULL) return;
        m_userBlockList.push_back(b);
    }
    
    void addGridBlock(rtBlock * b) {
        if(b == NULL) return;
        m_grid.push_back(b);
    }
    
    void display(SDL_Surface *surf, int offsetX, int offsetY) {
        for(int i = 0, k = 0; i < m_userBlockList.size(); i++) {
            (m_userBlockList[i])->display(surf, offsetX + (i%2)*rtBlock::WIDTH, offsetY + (k*rtBlock::HEIGHT));
            if(i%2 == 1) k++;
        }
        
        for(int i = 0; i < m_grid.size(); i++) {
            (m_grid[i])->display(surf, offsetX, offsetY);
        }
    }    
};

#endif

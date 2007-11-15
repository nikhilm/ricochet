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

#include "level.h"
#include "blocks.h"

rtLevel::rtLevel() : DOCK_OFFSET_X(DOCK_OFFSET_X_LOGICAL * rtBlock::WIDTH) {
    m_title = m_subtitle = m_passcode = "";
    m_userBlockList = std::vector<rtBlock *>();
    m_grid = std::vector<rtBlock *>();
    
    m_photon = NULL;
    m_switchesAlive = 0;
    
    m_activeBlock = NULL;
    m_dragInProgress = false;
    m_clicked = false;
    
    m_dockOut.x = DOCK_OFFSET_X;
    m_dockOut.y = DOCK_OFFSET_Y;
    
    m_dockIn.x = DOCK_OFFSET_X + DOCK_PADDING;
    m_dockIn.y = DOCK_OFFSET_Y + DOCK_PADDING;
}

void rtLevel::setTitle(std::string title) {
    m_title = title;
}

void rtLevel::setSubtitle(std::string stitle) {
    m_subtitle = stitle;
}

void rtLevel::setPasscode(std::string code) {
    m_passcode = code;
}

void rtLevel::addUserBlock(rtBlock * b) {
    if(b == NULL) return;
    m_userBlockList.push_back(b);
}

void rtLevel::addGridBlock(rtBlock * b) {
    if(b == NULL) return;
    
    b->registerLevel(this);    
    m_grid.push_back(b);
    
    if(b->type() == rtBlock::SWITCH)
        m_switchesAlive++;
}

void rtLevel::display(SDL_Surface *surf, int offsetX, int offsetY) {
    // dock
    m_dockOut.w = surf->w - m_dockOut.x;
    m_dockOut.h = surf->h;
    SDL_FillRect(surf, &m_dockOut, 0x666666);
    
    m_dockIn.w = surf->w - m_dockIn.x - DOCK_PADDING;
    m_dockIn.h = surf->h - 2 * DOCK_PADDING;
    SDL_FillRect(surf, &m_dockIn, 0x000000);
    
    for(int i = 0,
        x = m_dockIn.x - rtBlock::WIDTH/2 +m_dockIn.w/4,
        y = m_dockIn.y + 5*DOCK_PADDING;
        
        i < m_userBlockList.size(); i++) {
            
        (m_userBlockList[i])->display(surf, x + (i%DOCK_COLS)*(m_dockIn.w/2), y);
        
        if(i%DOCK_COLS == 1)
            y += rtBlock::HEIGHT + 2*DOCK_PADDING;
        
    }
    
    for(int i = 0; i < m_grid.size(); i++) {
        (m_grid[i])->display(surf, offsetX, offsetY);
    }
    
    if(m_photon != NULL)
        m_photon->display(surf, offsetX, offsetY);
}

void rtLevel::registerPhoton(rtPhoton * p) {
    m_photon = p;
}

void rtLevel::update() {
    if(m_photon != NULL) {
        for(int i = 0; i < m_grid.size(); ++i) {
            if(m_photon->x() == m_grid[i]->x()*rtBlock::WIDTH+rtBlock::WIDTH/2 &&
               m_photon->y() == m_grid[i]->y()*rtBlock::HEIGHT+rtBlock::HEIGHT/2)
                m_grid[i]->handlePhoton(*m_photon);
            
        }
        m_photon->move();
    }
}

bool rtLevel::handleEvent(SDL_Event evt) {
    if(evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT) {
        m_clicked = true;
        m_activeBlock = getBlockAt(evt.button.x, evt.button.y);
    }    
    else if(evt.type = SDL_MOUSEMOTION) {
        // react only if a drag is in progress
        if(m_clicked) {}
            
        int x = evt.button.x/rtBlock::WIDTH, y = evt.button.y/rtBlock::HEIGHT;
        for(int i = 0; i < m_grid.size(); ++i) {
            if(x == m_grid[i]->x() && y == m_grid[i]->y()) {
                return m_grid[i]->clicked();
            }
        }
    }
    return false;
}

// scans both grid and user list to see if block is below mouse
rtBlock * rtLevel::getBlockAt(int x, int y) {
    for(int i = 0; i < m_grid.size(); ++i) {
        if(x/rtBlock::WIDTH == m_grid[i]->x() && y/rtBlock::HEIGHT == m_grid[i]->y())
            return m_grid[i];
    }
    
    for(int i = 0; i < m_userBlockList.size(); ++i) {
        if(x/rtBlock::WIDTH == m_grid[i]->x() && y/rtBlock::HEIGHT == m_grid[i]->y())
            return m_userBlockList[i];
    }
    return NULL;
}

/***********
 * SIGNALS *
 **********/
 
void rtLevel::switchToggled(rtSwitch *sw) {
    ( sw->on() ? m_switchesAlive-- : m_switchesAlive++ );
    std::cout<<"Switches alive = "<<m_switchesAlive<<std::endl;
    if(m_switchesAlive == 0)
        gameOver(true);
}

// ends game
// if game over because player won success should be true
void rtLevel::gameOver(bool success) {
    if(success) {
        std::cout<<"~~~~ You won ~~~~\n";
        //winning code
    }
    else {
        std::cout<<"~~~~ You lost ~~~~\n";
        //loser code
    }
}

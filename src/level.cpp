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

rtLevel::rtLevel() : DOCK_OFFSET_X(DOCK_OFFSET_X_LOGICAL * rtBlock::WIDTH),
                 DOCK_START_X(DOCK_OFFSET_X + 2 * DOCK_PADDING), //start coordinate for block drawing
                 DOCK_START_Y(DOCK_OFFSET_Y + 2 * DOCK_PADDING)
{
    m_title = m_subtitle = m_passcode = "";
    m_userBlockList = std::vector<rtBlock *>();
    m_grid = std::vector<rtBlock *>();
    
    m_photon = NULL;
    m_switchesAlive = 0;
    
    m_activeBlock = NULL;
    m_dragInProgress = false;
    m_clicked = false;
    
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
    
    b->setX(m_userBlockList.size()%2);
    b->setY(m_userBlockList.size()/2);
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
    
    SDL_Rect out, in;
    
    //TODO:Replace 800/600 with the actual game resolution constants
    out.x = DOCK_OFFSET_X;
    out.y = DOCK_OFFSET_Y;
    out.w = 800 - out.x;
    out.h = 600;
    
    in.x = DOCK_OFFSET_X + DOCK_PADDING;
    in.y = DOCK_OFFSET_Y + DOCK_PADDING;
    in.w = 800 - in.x - DOCK_PADDING;
    in.h = 600 - 2 * DOCK_PADDING;
    
    SDL_FillRect(surf, &out, 0x666666);    
    SDL_FillRect(surf, &in, 0x000000);
    
    for(int i = 0; i < m_userBlockList.size(); i++) {
        (m_userBlockList[i])->display(surf, offsetX+getDockX(i), offsetY+getDockY(i));
    }
    
    for(int i = 0; i < m_grid.size(); i++) {
        (m_grid[i])->display(surf, offsetX, offsetY);
    }
    
    if(m_photon != NULL)
        m_photon->display(surf, offsetX, offsetY);
    
    if(m_activeBlock != NULL)
        m_activeBlock->display(surf, offsetX+m_activeBlockX, offsetY+m_activeBlockY);
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
        m_activeBlock = getBlockAt(evt.button.x, evt.button.y);
        if(m_activeBlock != NULL) {            
            m_clicked = true;
            m_activeBlock->setX(0);
            m_activeBlock->setY(0);
            m_activeBlockX = evt.button.x;
            m_activeBlockY = evt.button.y;
        }
    }    
    else if(evt.type == SDL_MOUSEMOTION) {
        // react only if a drag is to be begun
        if(m_clicked) {
            m_dragInProgress = true;            
        }
        
        if(m_dragInProgress) {            
            m_activeBlockX = evt.motion.x;
            m_activeBlockY = evt.motion.y;
        }
        
    }
    else if(evt.type == SDL_MOUSEBUTTONUP && evt.button.button == SDL_BUTTON_LEFT) {        
        m_clicked = false;
        
        if(!m_dragInProgress) {            
            int x = evt.button.x/rtBlock::WIDTH, y = evt.button.y/rtBlock::HEIGHT;
            for(int i = 0; i < m_grid.size(); ++i) {
                if(x == m_grid[i]->x() && y == m_grid[i]->y()) {
                    return m_grid[i]->clicked();
                }
            }
        }
        else {
            std::cout<<"Drag over\n";
            m_activeBlock == NULL;
            m_dragInProgress = false;
        }
    }
    return false;
}

// scans both grid and user list to see if block is below mouse
rtBlock * rtLevel::getBlockAt(int x, int y) {
    for(int i = 0; i < m_grid.size(); ++i) {
        if(x/rtBlock::WIDTH == m_grid[i]->x() && y/rtBlock::HEIGHT == m_grid[i]->y()) {
            rtBlock *tmp = m_grid[i];
            m_grid.erase(m_grid.begin()+i);
            return tmp;
        }
    }
    
    for(int i = 0; i < m_userBlockList.size(); ++i) {
        int bX = getDockX(i) + m_userBlockList[i]->x()*rtBlock::WIDTH;
        int bY = getDockY(i) + m_userBlockList[i]->y()*rtBlock::HEIGHT;
        
        if(bX <= x && bX + rtBlock::WIDTH >= x && bY <= y && bY + rtBlock::HEIGHT >= y) {
            rtBlock *tmp = m_userBlockList[i];
            m_userBlockList.erase(m_userBlockList.begin()+i);
            return tmp;
        }
    }
    return NULL;
}

// returns the position to draw the block at position pos in the list
// the best way to understand the dock calculations is to draw it on paper.
int rtLevel::getDockX(int pos) {
    return DOCK_START_X + DOCK_PADDING + (pos%DOCK_COLS) * (rtBlock::WIDTH);
}

int rtLevel::getDockY(int pos) {
    return DOCK_START_Y + DOCK_PADDING + (pos/DOCK_COLS) * (rtBlock::HEIGHT);
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

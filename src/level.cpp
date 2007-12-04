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

#include "blocks.h"
#include "game.h"

rtLevel::rtLevel() : DOCK_OFFSET_X(DOCK_OFFSET_X_LOGICAL * rtBlock::WIDTH),
                 DOCK_START_X(DOCK_OFFSET_X + 2 * DOCK_PADDING), //start coordinate for block drawing
                 DOCK_START_Y(DOCK_OFFSET_Y + 2 * DOCK_PADDING)
{
    m_title = m_subtitle = m_passcode = "";
    m_userBlockList = std::vector<rtBlock *>();
    m_grid = std::vector<rtBlock *>();
    
    m_photon = NULL;
    m_switchesAlive = 0;
    
    m_activeBlock = m_currentHandlingBlock = NULL;
    m_dragInProgress = false;
    m_clicked = false;
    m_currentHandlingBlockHandled = false;
    
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
    
    b->setX(getDockX(m_userBlockList.size()));
    b->setY(getDockY(m_userBlockList.size()));
    b->setDraggable(true);
    b->registerLevel(this);
    m_userBlockList.push_back(b);
}

void rtLevel::addGridBlock(rtBlock * b) {
    if(b == NULL) return;
    
    b->registerLevel(this);    
    m_grid.push_back(b);
    
    if(b->type() == rtBlock::SWITCH || b->type() == rtBlock::MULTI_SWITCH)
        m_switchesAlive++;
}

void rtLevel::display(SDL_Surface *surf) {
    SDL_FillRect(surf, NULL, 0x0);
    // dock  
    
    SDL_Rect out, in;
    
    out.x = DOCK_OFFSET_X;
    out.y = DOCK_OFFSET_Y;
    out.w = rtGame::SCREEN_WIDTH - out.x;
    out.h = rtGame::SCREEN_HEIGHT;
    
    in.x = DOCK_OFFSET_X + DOCK_PADDING;
    in.y = DOCK_OFFSET_Y + DOCK_PADDING;
    in.w = rtGame::SCREEN_WIDTH - in.x - DOCK_PADDING;
    in.h = rtGame::SCREEN_HEIGHT - 2 * DOCK_PADDING;
    
    SDL_FillRect(surf, &out, 0x666666);    
    SDL_FillRect(surf, &in, 0x000000);
    
    for(int i = 0; i < m_userBlockList.size(); i++) {
        (m_userBlockList[i])->display(surf, 0, 0);
    }
    
    for(int i = 0; i < m_grid.size(); i++) {
        (m_grid[i])->display(surf, 0, 0);
    }
    
    if(m_photon != NULL)
        m_photon->display(surf, 0, 0);
    
    if(m_activeBlock != NULL)
        m_activeBlock->display(surf, 0, 0);
}

void rtLevel::registerPhoton(rtPhoton * p) {
    m_photon = p;
}

void rtLevel::update() {    
    if(m_photon != NULL) {
        // level edge collisions
        if(m_photon->x() < 0 || m_photon->x() > GRID_WIDTH * rtBlock::WIDTH ||
           m_photon->y() < 0 || m_photon->y() > GRID_HEIGHT * rtBlock::HEIGHT) {
            killPhoton();
            return;
        }
        
        for(int i = 0; i < m_grid.size(); ++i) {
            // if it intersected the current block handling the photon is now m_grid[i]
            if(pointBlockIntersection(m_grid[i], m_photon->x(), m_photon->y())) {
                if(m_currentHandlingBlock != m_grid[i]) {
                    m_currentHandlingBlock = m_grid[i];
                    m_currentHandlingBlockHandled = false;
                }
                
            }
        }
        
        //if there is a block handling the photon and if it hasn't handled it already
        // (determined by the return value of the handlePhoton* methods)
        // then let it handle
        if(m_currentHandlingBlock && !m_currentHandlingBlockHandled) {
            //center
            if(m_photon->x() == m_currentHandlingBlock->x() + rtBlock::WIDTH/2 &&
               m_photon->y() == m_currentHandlingBlock->y() + rtBlock::HEIGHT/2) {
                if(m_currentHandlingBlock->handlePhoton(*m_photon))
                    m_currentHandlingBlockHandled = true;
            }
            else {
                std::cout<<"Handling sent to "<<m_currentHandlingBlock->type()<<std::endl;
                if(m_currentHandlingBlock->handlePhotonEdge(*m_photon)) {
                    m_currentHandlingBlockHandled = true;
                    std::cout<<"--Handled\n";
                }
            }
            if(m_photon)
                m_photon->move();
            return; // this is important, if we don't return the current handling block will become NULL
        }
        m_currentHandlingBlock = NULL;
        if(m_photon)
            m_photon->move();
    }
}

bool rtLevel::handleEvent(SDL_Event &evt) {
    //if a photon is active then nothing works
    if(m_photon) return false;
    
    if(evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT) {
        m_activeBlock = getBlockAt(evt.button.x, evt.button.y);
        if(m_activeBlock != NULL) {
            if(!m_activeBlock->draggable()) {
                // non draggable blocks are only present in the grid
                m_grid.push_back(m_activeBlock);
                m_activeBlock = false;
            }
            else {
                m_clicked = true;
            }
        }
    }    
    else if(evt.type == SDL_MOUSEMOTION) {
        // react only if a drag is to be begun
        if(m_clicked) {
            m_dragInProgress = true;            
        }
        
        if(m_dragInProgress) {    
            m_activeBlock->setX(evt.button.x - rtBlock::WIDTH/2);
            m_activeBlock->setY(evt.button.y - rtBlock::HEIGHT/2);
        }
        
    }
    else if(evt.type == SDL_MOUSEBUTTONUP && evt.button.button == SDL_BUTTON_LEFT) {        
        m_clicked = false;
        
        if(!m_dragInProgress) {
            for(int i = 0; i < m_grid.size(); ++i) {
                if(pointBlockIntersection(m_grid[i], evt.button.x, evt.button.y)) {
                    return m_grid[i]->clicked();
                }
            }
        }
        else {
            resolveDrop(m_activeBlock);
            m_activeBlock == NULL;
            m_dragInProgress = false;
        }
    }
    return false;
}

bool rtLevel::pointBlockIntersection(rtBlock *b, int x, int y) {
    return x >= b->x() && x <= b->x() + rtBlock::WIDTH &&
            y >= b->y() && y <= b->y() + rtBlock::HEIGHT;
}

// scans both grid and user list to see if block is below mouse
rtBlock * rtLevel::getBlockAt(int x, int y) {
    for(int i = 0; i < m_grid.size(); ++i) {
        if(pointBlockIntersection(m_grid[i], x, y)) {
            rtBlock *tmp = m_grid[i];
            m_grid.erase(m_grid.begin()+i);
            return tmp;
        }
    }
    
    for(int i = 0; i < m_userBlockList.size(); ++i) {
        if(pointBlockIntersection(m_userBlockList[i], x, y)) {
            rtBlock *tmp = m_userBlockList[i];
            m_userBlockList.erase(m_userBlockList.begin()+i);
            resetDockBlocks();
            return tmp;
        }
    }
    return NULL;
}

void rtLevel::resetDockBlocks() {
    for(int i = 0; i < m_userBlockList.size(); ++i) {
        m_userBlockList[i]->setX(getDockX(i));
        m_userBlockList[i]->setY(getDockY(i));
    }
}
        
void rtLevel::resolveDrop(rtBlock *b) {
    if(b->x() <= GRID_WIDTH * rtBlock::WIDTH && b->y() <= GRID_HEIGHT * rtBlock::HEIGHT) {
        //integer division truncation helps
        b->setX((b->x()+rtBlock::WIDTH/2)/rtBlock::WIDTH * rtBlock::WIDTH);
        b->setY((b->y()+rtBlock::HEIGHT/2)/rtBlock::HEIGHT * rtBlock::HEIGHT);
        m_grid.push_back(b);
    }
    else {
        addUserBlock(b);
    }
}

// returns the position to draw the block at position pos in the list
// the best way to understand the dock calculations is to draw it on paper.
int rtLevel::getDockX(int pos) {
    return DOCK_START_X + DOCK_PADDING + (pos%DOCK_COLS) * (DOCK_PADDING*3 + rtBlock::WIDTH);
}

int rtLevel::getDockY(int pos) {
    return DOCK_START_Y + DOCK_PADDING + (pos/DOCK_COLS) * (DOCK_PADDING*3 + rtBlock::HEIGHT);
}

/***********
 * SIGNALS *
 **********/
 
void rtLevel::switchToggled(rtSwitch *sw) {
    ( sw->on() ? m_switchesAlive-- : m_switchesAlive++ );
    std::cout<<"Switches alive = "<<m_switchesAlive<<std::endl;
    if(m_switchesAlive == 0)
        levelDone();
}

// ends game
// if game over because player won success should be true
void rtLevel::levelDone() {
    rtGame::changeState(new rtTransitionState(m_levelNumber));    
}

// kills the photon and resets all switches to off
// TODO:segfault bug
void rtLevel::killPhoton() {
    std::cout<<m_photon->type()<<std::endl;
    delete m_photon;
    m_photon = NULL;
    for(int i = 0; i < m_grid.size(); ++i) {
        if(m_grid[i]->type() == rtBlock::SWITCH || m_grid[i]->type() == rtBlock::MULTI_SWITCH) {
            rtSwitch *sw = (rtSwitch *) m_grid[i];
            if(sw->on())
                sw->toggleState();
        }
    }
}

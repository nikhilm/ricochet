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

rtLevel::rtLevel() {
    m_title = m_subtitle = m_passcode = "";
    m_userBlockList = std::vector<rtBlock *>();
    m_grid = std::vector<rtBlock *>();
    
    m_photon = NULL;
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
}

void rtLevel::display(SDL_Surface *surf, int offsetX, int offsetY) {
    if(m_photon != NULL)
        m_photon->display(surf, offsetX, offsetY);
    for(int i = 0, k = 0; i < m_userBlockList.size(); i++) {
        (m_userBlockList[i])->display(surf, offsetX + (i%2)*rtBlock::WIDTH, offsetY + (k*rtBlock::HEIGHT));
        if(i%2 == 1) k++;
    }
    
    for(int i = 0; i < m_grid.size(); i++) {
        (m_grid[i])->display(surf, offsetX, offsetY);
    }
}

void rtLevel::registerPhoton(rtPhoton * p) {
    m_photon = p;
}

void rtLevel::update() {
    if(m_photon != NULL)
        m_photon->move();
}

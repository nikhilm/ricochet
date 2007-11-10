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

#ifndef RT_EDITOR_H
#define RT_EDITOR_H

#include <iostream>
#include <string>
#include <vector>

#include "resource.h"
#include "level.h"
#include "blocks.h"

struct rtBlkWrp {
    rtBlock * block;
    bool userControlled;
};

class rtEditor {
    rtBlkWrp grid[rtLevel::GRID_WIDTH][rtLevel::GRID_HEIGHT];
    
public:
    rtEditor
};

int main(int argc, char ** argv) {
    if(argc != 5) {
        std::cout<<"Usage: "<<argv[0]<<" <file> <title> <subtitle> <passcode>\n";
        return 0;
    }
    
    rtEditor(argv[1], argv[2], argv[3], argv[4]);
    return 0;
}

#endif

/*
 * Copyright (C)      2007  Nikhil Marathe <nsm.nikhil@gmail.com>
 *               
 * This file is part of the Graphyte project.
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

#ifndef RT_LEVELPARSER_H
#define RT_LEVELPARSER_H

#include <iostream>

#include "resource.h"
#include "level.h"
#include "blocks.h"

class rtLevelParser {
    static std::vector<Level> Levels = std::vector<Level>();
public:
    void init() {
        std::ifstream levelFile = rtResource::getFile("levels.txt");
        
        if(levelFile == NULL || levelFile.bad()) {
            std::cerr<<"Could not read levels\n";
            return;
        }
    
        Level curLevel;
        while(levelFile) {
            if(levelFile.eof()) break;
            
            char * line;
            
            //read in the 
            levelFile.getline(line, 100);
            
        }
    }
};

#endif

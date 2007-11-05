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

#ifndef RT_LEVELS_H
#define RT_LEVELS_H

#include <iostream>
#include <string>
#include <map>

#include <SDL/SDL.h>

Levels = std::vector<Level>();
        std::ifstream levelFile(RT_DATA + "levels.txt");
        if(levelFile.bad()) {
            std::cerr<<"Could not read levels\n";
            return;
        }
        
        Level curLevel;
        while(levelFile) {
            if(levelFile.eof()) break;
            
            char * line;
            
            //read in the 
            levelFile.getline(line, 100);
            sw
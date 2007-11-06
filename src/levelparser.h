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
#include <string>
#include <vector>

#include "resource.h"
//#include "level.h"
#include "blocks.h"

/*
 * In init levels are parsed and stored as a quick struct.
 * Its only when the level is requested in makeLevel that
 * the actual objects are generated.
 */
 
struct rtFakeLevel {
    std::string title, subtitle, passcode, userListStr, gridStr;
};


static std::vector<rtFakeLevel> Levels;
class rtLevelParser {
public:
    static void init() {
        Levels = std::vector<rtFakeLevel>();
        
        std::ifstream levelFile(rtResource::getFile("levels.txt"));
        
        if(levelFile == NULL || levelFile.bad()) {
            std::cerr<<"Error opening levels file."<<rtResource::getFile("levels.txt")<<std::endl;
        }
        
        rtFakeLevel curLevel;
        curLevel.gridStr = "";
        while(levelFile) {
            if(levelFile.eof()) break;
            
            char line[100];
            
            //read in the 
            levelFile.getline(line, 100);
            
            //empty or length less than 2 but not a '!'
            if(strlen(line) > 0  && line[0] == '!') {
                Levels.push_back(curLevel);
                std::cout<<Levels.size()<<std::endl;
                curLevel.gridStr = "";
                continue;
            }
            
            if(strlen(line) < 3) continue;
            
            std::string substring = std::string(line).substr(2);
            switch(line[0]) {
                case '#':break; //comment
                
                case '>':
                    curLevel.title = substring;
                    break;
                    
                case '<':
                    curLevel.subtitle = substring;
                    break;
                
                case '&':
                    curLevel.passcode = substring;
                    break;
                
                case '@':
                    curLevel.userListStr = substring;
                    break;
                    
                default: //load the grid
                    curLevel.gridStr += line;
                    curLevel.gridStr += "\n";
            }
            
            
        }
        
        for(int i = 0; i < Levels.size(); ++i) {
            rtFakeLevel tmp = Levels[i];
            std::cout<<"Level:"<<tmp.title<<std::endl;
            std::cout<<"\tSubtitle:"<<tmp.subtitle<<std::endl;
            std::cout<<"\tuserList:"<<tmp.userListStr<<std::endl;
            std::cout<<"\tgridStr:"<<tmp.gridStr<<std::endl;
        }
    }
};

#endif

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

#ifndef RT_LEVELPARSER_H
#define RT_LEVELPARSER_H

#include <iostream>
#include <string>
#include <vector>

#include "resource.h"
#include "level.h"
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
    static rtBlock * getBlock(char type, char d, int x, int y) {
        int dir = 0;
        switch(d) {
            case 'U':
                dir = rtBlock::UP;
                break;
            case 'D':
                dir = rtBlock::DOWN;
                break;
            case 'L':
                dir = rtBlock::LEFT;
                break;
            case 'R':
                dir = rtBlock::RIGHT;
                break;
        }
        
        std::cout<<"Recd x "<<x<<" y "<<y<<std::endl;
        switch(type) {
            case 'a':
                return(new rtArrow(dir, x, y));
                
                
            case 'b':
                return(new rtBomb(dir, x, y));
                
                
            case 'd':
                return(new rtDeflector(dir, x, y));
                
                
            case 'l':
                return(new rtLauncher(dir, x, y));
                
                
            case 'p':
                return(new rtPrism(dir, x, y));
                
                
            case 's':
                return(new rtSwitch(dir, x, y));
                
                
            case 'w':
                return(new rtWall(dir, x, y));
                
                
            case 'x':
                return(new rtEnergiser(dir, x, y));
                
            default:
                return NULL;    
        }
    }
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
    
    static rtLevel * getLevel(int num) {
        if(num >= Levels.size() || num < 0) {
            std::cerr<<"No level number "<<num<<std::endl;
            return NULL;
        }
        
        rtFakeLevel fake = Levels[num%Levels.size()];
        
        rtLevel * level = new rtLevel;
        
        level->setTitle(fake.title);
        level->setSubtitle(fake.subtitle);
        level->setPasscode(fake.passcode);
        //user list
        for(int i = 0; i < fake.userListStr.length(); i+=2) {
            level->addUserBlock(getBlock(fake.userListStr[i], fake.userListStr[i+1], 0, 0));
        }
        
        //grid list
        for(int i = 0, x = 0, y = 0; i < fake.gridStr.length(); i++, x++) {
            if(fake.gridStr[i] == '\n') {
                y++;
                x = 0;
                continue;
            }
            if(fake.gridStr[i] == 'e') {
                i++;
                continue;
            }
            
            level->addGridBlock(getBlock(fake.gridStr[i], fake.gridStr[i+1], x, y));
            i++;
        }
        
        return level;
    }
};

#endif

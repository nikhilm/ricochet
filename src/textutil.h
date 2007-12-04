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

#ifndef RT_TEXTUTIL_H
#define RT_TEXTUTIL_H

#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "resource.h"


class rtTextUtil {
    
    static const char * RT_STD_FONT;
    
    static SDL_Surface * render(const char *, SDL_Color, TTF_Font *, bool bold=false, bool italic=false);
    
    //splits string everytime it is called
    //original string is modified!
    static std::string splitString(std::string & str, char delim='\n');
    
public:

    static TTF_Font * RT_LARGE_FONT;
    static TTF_Font * RT_MEDIUM_FONT;
    static TTF_Font * RT_SMALL_FONT;
    
    enum {ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT};
    
    static bool init();
    
    static void render(const char *,
                         SDL_Color,
                         TTF_Font *,
                         SDL_Surface *,
                         int ,
                         int ,
                         int align=ALIGN_LEFT,
                         bool bold=false,
                         bool italic=false,
                         int yPadding=5);
    
    static void cleanup();
};

#endif

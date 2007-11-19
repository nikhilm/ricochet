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

static const char * RT_STD_FONT = "DejaVuSans.ttf";

static TTF_Font * RT_LARGE_FONT;
static TTF_Font * RT_SMALL_FONT;

class rtTextUtil {
public:
    
    static bool init() {
        if(TTF_Init()==-1) {
            std::cout<<"TTF_Init: "<<TTF_GetError()<<std::endl;
            return false;
        }
        //load default fonts
        RT_LARGE_FONT = TTF_OpenFont(rtResource::getFile(RT_STD_FONT), 40);
        if(RT_LARGE_FONT == NULL) {
            std::cout<<"TTF_OpenFont: "<<TTF_GetError()<<std::endl;
            return false;
        }
        
        RT_SMALL_FONT = TTF_OpenFont(rtResource::getFile(RT_STD_FONT), 14);
        if(RT_SMALL_FONT == NULL) {
            std::cout<<"TTF_OpenFont: "<<TTF_GetError()<<std::endl;
            return false;
        }       
        
        return true;
        
    }
    
    static SDL_Surface * render(const char * text, SDL_Color color, TTF_Font * font, bool bold=false, bool italic=false) {
        SDL_Surface *text_surface;
        
        TTF_SetFontStyle(font, ( bold ? TTF_STYLE_BOLD : TTF_STYLE_NORMAL ) | ( italic ? TTF_STYLE_ITALIC : TTF_STYLE_NORMAL ));
        
        if(!(text_surface=TTF_RenderText_Blended(font, text, color))) {
            return NULL;
        }
        else {
            return text_surface;
        }
    }
    
    static void cleanup() {
        TTF_CloseFont(RT_LARGE_FONT);
        TTF_CloseFont(RT_SMALL_FONT);
    }
};

#endif

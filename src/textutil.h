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
static TTF_Font * RT_MEDIUM_FONT;
static TTF_Font * RT_SMALL_FONT;

class rtTextUtil {
    
    static SDL_Surface * render(const char * text, SDL_Color color, TTF_Font * font, bool bold=false, bool italic=false) {
        SDL_Surface *text_surface;
        
        TTF_SetFontStyle(font, ( bold ? TTF_STYLE_BOLD : TTF_STYLE_NORMAL ) | ( italic ? TTF_STYLE_ITALIC : TTF_STYLE_NORMAL ));
        
        if(!(text_surface=TTF_RenderText_Solid(font, text, color))) {
            return NULL;
        }
        else {
            return text_surface;
        }
    }
    
    //splits string everytime it is called
    //original string is modified!
    static std::string splitString(std::string & str, char delim='\n') {
        if(str.empty()) return str;
        
        int pos = str.find_first_of(delim);
        
        if(pos == std::string::npos) {
            std::string copy = str;
            str = "";
            return copy;
        }
        
        std::string ret = str.substr(0, pos);
        
        str = str.substr(pos+1);
        
        return ret;
    }
    
public:
    
    enum {ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT};
    
    static bool init() {
        if(TTF_Init()==-1) {
            std::cout<<"TTF_Init: "<<TTF_GetError()<<std::endl;
            return false;
        }
        //load default fonts
        RT_LARGE_FONT = TTF_OpenFont(rtResource::getFile(RT_STD_FONT), 50);
        if(RT_LARGE_FONT == NULL) {
            std::cout<<"TTF_OpenFont: "<<TTF_GetError()<<std::endl;
            return false;
        }
        
        RT_MEDIUM_FONT = TTF_OpenFont(rtResource::getFile(RT_STD_FONT), 30);
        if(RT_MEDIUM_FONT == NULL) {
            std::cout<<"TTF_OpenFont: "<<TTF_GetError()<<std::endl;
            return false;
        }
        
        RT_SMALL_FONT = TTF_OpenFont(rtResource::getFile(RT_STD_FONT), 20);
        if(RT_SMALL_FONT == NULL) {
            std::cout<<"TTF_OpenFont: "<<TTF_GetError()<<std::endl;
            return false;
        }
        return true;
        
    }
    
    /**
     * Renders given text on the surface.
     * NOTE: Avoid calling in a loop. Blended rendering is slow
     * 
     * @param text The text to render
     * @param color Color to render text in
     * @param font Choose from RT_LARGE_FONT, RT_MEDIUM_FONT and RT_SMALL_FONT
     * @param dest The destination surface to blit to
     * @param x The x coordinate of destination where text will be blitted
     * @param y The y coordinate of destination where text will be blitted
     * @param align How to align the text. Valid values are rtTextUtil::ALIGN_LEFT (default), rtTextUtil::ALIGN_CENTER, rtTextUtil::ALIGN_RIGHT. The x, y coordinates are relative to position.
     * @param bold Render bold text (Default false)
     * @param italic Render italic text (Default false)
     * @param yPadding The distance between two consecutive lines.
     */
    static void render(const char * text,
                         SDL_Color color,
                         TTF_Font * font,
                         SDL_Surface * dest,
                         int x,
                         int y,
                         int align=ALIGN_LEFT,
                         bool bold=false,
                         bool italic=false,
                         int yPadding=5) {
                             
        //split by newlines and generate new surfaces for each
        std::string txt = text;
        std::string token;
        SDL_Surface *txtSurf;
        
        SDL_Rect r;
        r.y = y-TTF_FontHeight(font)/2;  
        
        while(!txt.empty()) {
            token = splitString(txt);
            if(token.empty())
                break;
            txtSurf = render(token.c_str(), color, font, bold, italic);
            
            if(txtSurf == NULL) {
                std::cerr<<"Error rendering text: "<<TTF_GetError()<<std::endl;
                return;
            }
            
            switch(align) {
                case ALIGN_LEFT:
                    r.x = x;
                    break;
                case ALIGN_RIGHT:
                    r.x = x - txtSurf->w;
                    break;
                case ALIGN_CENTER:
                    r.x = x - txtSurf->w/2;
                    break;
            }
            SDL_BlitSurface(txtSurf, NULL, dest, &r);
            SDL_FreeSurface(txtSurf);
           
            r.y += TTF_FontHeight(font) + yPadding;
            //std::cout<<"Y is now "<<y<<std::endl;
        }
    }
    
    static void cleanup() {
        TTF_CloseFont(RT_LARGE_FONT);
        TTF_CloseFont(RT_MEDIUM_FONT);
        TTF_CloseFont(RT_SMALL_FONT);
    }
};

#endif

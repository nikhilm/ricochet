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

#ifndef RT_RESOURCE_H
#define RT_RESOURCE_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

const std::string RT_DATA = "./";
const std::string RT_IMGEXT = ".png";

static std::map<std::string, SDL_Surface *> ImageCache;

class rtResource {
public:
    static void init() {
        ImageCache = std::map<std::string, SDL_Surface *>();    
    }
    
    /** 
     * tries to load image RT_DATA/images/prefix_root_suffix.png
     * If that fails tries RT_DATA/images/prefix_root.png
     * Otherwise returns NULL.
     */
    static SDL_Surface * loadImage(std::string prefix, std::string root, std::string suffix = "") {
        SDL_Surface * image;
        
        std::string file = RT_DATA + "images/" + prefix + "_" + root + "_" + suffix + RT_IMGEXT;
        if(ImageCache.find(file) != ImageCache.end()) {
            return ImageCache[file];
        }
        
        image = IMG_Load(file.c_str());
                
        if(image == NULL) {
            std::string file2 = RT_DATA + "images/" + prefix + "_" + root + RT_IMGEXT;
            
            if(ImageCache.find(file2) != ImageCache.end()) {
                return ImageCache[file2];
            }
            
            image = IMG_Load(file2.c_str());
            
            if(image == NULL) {
                std::cerr<<"IMG_Load: "<<IMG_GetError()<<std::endl;
                return NULL;
            }
            else {
                ImageCache[file2] = image;
                return ImageCache[file2];
            }
        }        
        else {
            ImageCache[file] = image;
            return ImageCache[file];
        }
    }
    
    static std::ifstream getFile(const char * fileName) {
        std::ifstream file(RT_DATA+fileName);
        if(file.bad()) {
            std::cerr<<"Error opening file "<<fileName<<std::endl;
            return NULL;
        }
        return file;
    }
    
    static void cleanup() {
        std::map<std::string, SDL_Surface *>::iterator it;
        for(it = ImageCache.begin(); it != ImageCache.end(); it++) {
            SDL_FreeSurface(it->second);
            it->second = NULL;
        }
    }
};

#endif

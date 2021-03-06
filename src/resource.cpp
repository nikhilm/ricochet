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
 
#include "resource.h"

bool rtResource::init() {
    ImageCache = std::map<std::string, SDL_Surface *>();
    return true;
}

/** 
 * tries to load image RT_DATA/images/prefix_root_suffix.png
 * If that fails tries RT_DATA/images/prefix_root.png
 * Otherwise returns NULL.
 */
SDL_Surface * rtResource::loadImage(std::string prefix, std::string root, std::string suffix) {
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

const char * rtResource::getFile(const char * fileName) {
    return (RT_DATA+fileName).c_str();
}

void rtResource::cleanup() {
    std::map<std::string, SDL_Surface *>::iterator it;
    for(it = ImageCache.begin(); it != ImageCache.end(); it++) {
        SDL_FreeSurface(it->second);
        it->second = NULL;
    }
}

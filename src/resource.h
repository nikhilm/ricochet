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

#ifndef RT_RESOURCE_H
#define RT_RESOURCE_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

const std::string RT_DATA = "./data/";
const std::string RT_IMGEXT = ".png";

static std::map<std::string, SDL_Surface *> ImageCache;

class rtResource {
public:
    static void init();
    
    /** 
     * tries to load image RT_DATA/images/prefix_root_suffix.png
     * If that fails tries RT_DATA/images/prefix_root.png
     * Otherwise returns NULL.
     */
    static SDL_Surface * loadImage(std::string prefix, std::string root, std::string suffix = "");
    
    static const char * getFile(const char *);
    
    static void cleanup();
};

#endif

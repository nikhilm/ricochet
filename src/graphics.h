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
#ifndef GRAPHYTE_GRAPHICS_H
#define GRAPHYTE_GRAPHICS_H

#include <iostream>
#include <cmath>

#include <SDL/SDL.h>

const float PI = 3.14159265;

/**
 * Wraps SDL_Color to give some more functionality
 * @author Nikhil Marathe
 */
class Color {
private:
    SDL_Color color;
    
public: 
    /**
* Creates a Color object set to black
    */
    Color() {
        setR(0);
        setG(0);
        setB(0);
    }

    /**
    * Creates a Color object from the (r, g, b) values.
    */
    Color(int r, int g, int b) {
        setR(r);
        setG(g);
        setB(b);
    }
    
    /**
    * Copy constructor.
    */
    Color(const Color& other) {
        setR(other.r());
        setG(other.g());
        setB(other.b());
    }

    Uint8 r() const { return color.r; }
    Uint8 g() const { return color.g; }
    Uint8 b() const { return color.b; }

    void setR(int r) { color.r = r; }
    void setG(int g) { color.g = g; }
    void setB(int b) { color.b = b; }

    /**
    * Returns the Uint32 form of this Color.
    * This can be passed to SDL functions.
    * 
    * @param fmt The SDL_PixelFormat pointer.
    * @see SDL_MapRGB
    */
    Uint32 toUint32(SDL_PixelFormat *fmt) { return SDL_MapRGB(fmt, r(), g(), b()); }
    
    /**
    * Takes an SDL_Surface and returns a Uint32 representation of this Color using the surface's pixel format.
    * 
    * @param surf The SDL_Surface pointer.
    * @see toUint32(SDL_PixelFormat *)
    */
    Uint32 toUint32(SDL_Surface *surf) { return SDL_MapRGB(surf->format, r(), g(), b()); }

};

/**
    * Provides a wrapper to an SDL_Surface to quickly draw various shapes on it.
    */
class Graphics {
    
    SDL_Surface * surface;
    
    Color currentColour;
    /**
        * Locks the surface
        */
    void lock() {
        if(SDL_MUSTLOCK(surface)) {
            if(SDL_LockSurface(surface) < 0) { return; }
            else { std::cerr<<"Graphyte::Graphics error: Could not lock surface\n"<<SDL_GetError(); }
        }
    }

    void unlock() {
        SDL_UnlockSurface(surface);
    }
    
    /**
        * Fills the pixel with the specified colour.
        * Assumes the caller will lock and unlock the surface, so be careful.
        */
    void fillPixel(int x, int y) {
        //std::cout<<"Coords:"<<drawX<<", "<<drawY<<"   Actual coords:"<<x<<", "<<y<<std::endl;
        //bounds checking
        if(x < 0 || y < 0 || x >= surface->w || y >= surface->h) return;
    
        Uint32 pixel = currentColour.toUint32(surface);

        /* This part is shamelessly ripped from the SDL guide */

        int bpp = surface->format->BytesPerPixel;
        /* Here p is the address to the pixel we want to set */
        Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    
        switch(bpp) {
            case 1:
                *p = pixel;
                break;
    
            case 2:
                *(Uint16 *)p = pixel;
                break;
    
            case 3:
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                    p[0] = (pixel >> 16) & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = pixel & 0xff;
                } else {
                    p[0] = pixel & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = (pixel >> 16) & 0xff;
                }
                break;
    
            case 4:
                *(Uint32 *)p = pixel;
                break;
        }
        /* End Rip */
    }
    
public:
    /**
        * Creates a new Graphics object with the surface surf.
        */
    Graphics(SDL_Surface * surf) {
        surface = surf;
        currentColour = Color(0, 0, 0);
    }
    
    SDL_Surface * getSurface() const {
        return surface;
    }
    
    /**
        * Sets the fill colour to the Color object.
        */
    void setColor(const Color& colour) {
        currentColour = colour;
    }
    
    /**
        * Sets the fill colour to the RGB value.
        */
    void setColor(int r, int g, int b) {
        setColor(Color(r, g, b));
    }
    
    
    /**
        * Fills pixel (x, y) with current colour.
        */
    void setPixel(int x, int y) {
        lock();
        fillPixel(x, y);
        unlock();
    }
    
    /**
        * Draws an arc from startAngle, arcAngle radians. The center of the arc is at (x, y) and
        * its width is width and height is height.
        */
    void drawArc(int x, int y, int width, int height, double startAngle, double arcAngle) {
        lock();
        if(arcAngle < 0) {
            for(double i = startAngle; i >= startAngle+arcAngle; i -= 0.01)
                fillPixel((int)(x + width*cos(i)), (int)(y + height*sin(i)));
        }
        else {
            for(double i = startAngle; i <= startAngle+arcAngle; i += 0.01)
                fillPixel((int)(x + width*cos(i)), (int)(y + height*sin(i)));
        }
        unlock();
    }
    
    /**
        * Draw a oval with centre (centerX, centerY) and major axis radiusA, minor axis radiusB
        */
    void drawOval(int centerX, int centerY, int width, int height) {
        drawArc(centerX, centerY, width, height, 0, PI*2);
    }
    
    /**
        * Draws a thick oval.
        * @see drawOval
        */
    void drawOval(int centerX, int centerY, int width, int height, int thickness) {
        if(thickness < 1) {
            std::cout<<"Thickness of circle should be >= 1\n";
            return;
        }
        for(int i = 1; i <= thickness; ++i)
            drawOval(centerX, centerY, width + i, height + i);
    }
    
};

#endif

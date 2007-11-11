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

#ifndef RT_BLOCKS_H
#define RT_BLOCKS_H

#include <iostream>

#include <SDL/SDL.h>

class rtResource;
class rtLevel;
class rtPhoton;
/*
 * Main block class. See Trac wiki entry for more information
 * Make sure you register a level at appropriate times, there are no checks here mainly because checking at every access would be a pain.
 */
class rtBlock {
protected:
    int m_x, m_y;
    
    SDL_Surface * m_image;
    
    char m_type;
    
    int m_direction;
    
//     Level * m_level;
    
    bool m_draggable;

    void setDraggable(bool d) {
        m_draggable = d;
    }
    
    void setImage(char type='\0', const char * suffix=NULL) {
        m_image = rtResource::loadImage("block", (type == '\0' ? m_type +"" : type+""), (suffix == NULL ? directionToString() : suffix));
        if(m_image == NULL) {
            std::cerr<<"Could not load block "<<type<<" image\n";
        }
    }
    
public:
    enum Direction {UP, LEFT, DOWN, RIGHT};
    enum {WIDTH = 50, HEIGHT = 50};
    
    rtBlock(char type, int dir, int x, int y) {
        m_type = type;
        setDirection(dir);
        setX(x);
        setY(y);
        setDraggable(false);
    }
    
    rtBlock(const rtBlock & b) {
        m_type = b.m_type;
        setX(b.x());
        setY(b.y());
        setDraggable(b.draggable());
        m_image = b.m_image;
        setDirection(b.direction());
    }
    
    int x() const { return m_x; }
    int y() const { return m_y; }
    
    virtual void setX(int x) {
//         if(x < 0 || x > rtLevel::WIDTH) return;
        m_x = x;
    }
    
    virtual void setY(int y) {
//         if(y < 0 || y > rtLevel::HEIGHT) return;
        m_y = y;
    }
    
//     void registerLevel(Level * lvl) {
//         m_level = lvl;
//     }
    
    bool draggable() const { return m_draggable; }
    
    virtual void setDirection(int dir) {
        m_direction = dir;
        setImage();
    }
    
    int direction() const { return m_direction; }
    
    const char * directionToString() const {
        switch(direction()) {
            case UP: return "U";
            case DOWN: return "D";
            case LEFT: return "L";
            case RIGHT: return "R";
            default: return "";
        }
    }
    
    virtual void display(SDL_Surface *surf, int offsetX, int offsetY) const {
        SDL_Rect r;
        r.x = offsetX + m_x * WIDTH;
        r.y = offsetY + m_y * HEIGHT;
        SDL_BlitSurface(m_image, NULL, surf, &r);
    }
    
    virtual bool handlePhoton(rtPhoton &photon) const {
        return false;
    }
    
    virtual bool clicked() {
        return false;
    }
};

class rtPhoton : public rtBlock {
    bool m_highEnergy;
    
    int m_deltaX, m_deltaY;
public:
    rtPhoton(int dir, int x, int y) : rtBlock('o', dir, x, y) {
        setHighEnergy(false);
        setDirection(dir);
    }
    void setX(int x) {
//         if(x < 0 || x > rtLevel::WIDTH * WIDTH) return;
        m_x = x;
    }
    
    void setY(int y) {
//         if(y < 0 || y > rtLevel::HEIGHT * HEIGHT) return;
        m_y = y;
    }
    
    void setHighEnergy(bool he) {
        m_highEnergy = he;
        std::cout<<"High energy:"<<(highEnergy() ? "yes\n":"no\n");
        setImage('o', (highEnergy() ? "energised" : ""));
    }
    
    bool highEnergy() {
        return m_highEnergy;
    }
    
    void setDirection(int dir) {
        //if high energy, reduce energy but keep direction the same
        if(highEnergy()) {
            setHighEnergy(false);
            return;
        }
        m_direction = dir;
        switch(direction()) {
            case UP:m_deltaX = 0;m_deltaY = -1; break;
            case DOWN: m_deltaX = 0; m_deltaY = 1; break;
            case LEFT: m_deltaX = -1; m_deltaY = 0; break;
            case RIGHT: m_deltaX = 1; m_deltaY = 0; break;
        }
    }
    
    void move() {
        setX(x() + m_deltaX);
        setY(y() + m_deltaY);
    }
    
    void display(SDL_Surface * surf, int offsetX, int offsetY) {
        SDL_Rect r;
        r.x = offsetX + x()-m_image->w/2;
        r.y = offsetY + y()-m_image->h/2;
        r.w = m_image->w;
        r.h = m_image->h;
        SDL_FillRect(surf, &r, 0x0);
        SDL_BlitSurface(m_image, NULL, surf, &r);
    }
};

class rtLauncher : public rtBlock {
public:
    rtLauncher(int dir, int x, int y) : rtBlock('l', dir, x, y) {}
    
    bool clicked() {
        //m_level->registerPhoton();
        std::cout<<"Created photon\n";
        return true;
    }
};

class rtSwitch : public rtBlock {
    bool m_on;
    
    void toggleState() {
        m_on = !m_on;
        setImage('s', (m_on ? "on" : "off"));
    }
    
public:
    rtSwitch(int dir, int x, int y) : rtBlock('s', dir, x, y) {
        m_on = true;
        toggleState();
    }
    bool handlePhoton(rtPhoton &photon) {
        photon.setHighEnergy(false);
        toggleState();
        return true;
    }
    
    bool on() { return m_on; }
};

class rtBomb : public rtBlock {
public:
    rtBomb(int dir, int x, int y) : rtBlock('b', dir, x, y) {}
    
    bool handlePhoton(rtPhoton &photon) {
        //m_level->triggerGameOver();
        return true;
    }
};

class rtWall : public rtBlock {
public:
    rtWall(int dir, int x, int y) : rtBlock('w', dir, x, y) {}
    
    bool handlePhoton(rtPhoton &photon) {
        /*switch(photon.direction()) {
            case UP:photon.setDirection(DOWN); break;
            case DOWN:photon.setDirection(UP); break;
            case LEFT:photon.setDirection(RIGHT): break;
            case RIGHT:photon,setDirection(LEFT): break;
        }
        return true;*/
        //m_level->triggerKillPhoton();
        return true;
    }
};

class rtDeflector : public rtBlock {
public:
    rtDeflector(int dir, int x, int y) : rtBlock('d', dir, x, y) {
        setDraggable(true);
    }
    
    // you should take a look at the images to understand this
    bool handlePhoton(rtPhoton &photon) {
        bool deflected = true;
        switch(direction()) {
            case UP:
                if(photon.direction() == DOWN)
                    photon.setDirection(RIGHT);
                else if(photon.direction() == LEFT)
                    photon.setDirection(UP);
                else
                    deflected = false;
                break;
            case DOWN:
                if(photon.direction() == UP)
                    photon.setDirection(LEFT);
                else if(photon.direction() == RIGHT)
                    photon.setDirection(DOWN);
                else
                    deflected = false;
                break;
            case LEFT:
                if(photon.direction() == RIGHT)
                    photon.setDirection(UP);
                else if(photon.direction() == DOWN)
                    photon.setDirection(LEFT);
                else
                    deflected = false;
                break;
            case RIGHT:
                if(photon.direction() == LEFT)
                    photon.setDirection(DOWN);
                else if(photon.direction() == UP)
                    photon.setDirection(RIGHT);
                else
                    deflected = false;
                break;
        }
        if(!deflected) {
            ;//m_level->triggerKillPhoton();
            photon.setHighEnergy(false);
        }
        return true;
    }
};

class rtPrism : public rtBlock {
public:
    rtPrism(int dir, int x, int y) : rtBlock('p', dir, x, y) {
        setDraggable(true);
    }
    
    bool handlePhoton(rtPhoton &photon) {
        switch(direction()) {
            case UP:
                if(photon.direction() == DOWN)
                    photon.setDirection(RIGHT);
                else if(photon.direction() == LEFT)
                    photon.setDirection(UP);
                else
                    photon.setHighEnergy(false);
                break;
            case DOWN:
                if(photon.direction() == UP)
                    photon.setDirection(LEFT);
                else if(photon.direction() == RIGHT)
                    photon.setDirection(DOWN);
                else
                    photon.setHighEnergy(false);
                break;
            case LEFT:
                if(photon.direction() == RIGHT)
                    photon.setDirection(UP);
                else if(photon.direction() == DOWN)
                    photon.setDirection(LEFT);
                else
                    photon.setHighEnergy(false);
                break;
            case RIGHT:
                if(photon.direction() == LEFT)
                    photon.setDirection(DOWN);
                else if(photon.direction() == UP)
                    photon.setDirection(RIGHT);
                else
                    photon.setHighEnergy(false);
                break;
        }
    }
};

class rtArrow : public rtBlock {
public:
    rtArrow(int dir, int x, int y) : rtBlock('a', dir, x, y) {
        setDraggable(true);
    }
    
    bool handlePhoton(rtPhoton &photon) {
        photon.setDirection(direction());
    }
};

class rtEnergiser : public rtBlock {
public:
    rtEnergiser(int dir, int x, int y) : rtBlock('x', dir, x, y) {
        setDraggable(true);
    }
    
    bool handlePhoton(rtPhoton &photon) {
        std::cout<<"Handle photon called\n";
        photon.setHighEnergy(true);
    }
};

#endif

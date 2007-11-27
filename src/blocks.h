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

#include "resource.h"
#include "level.h"

char charDirection(int);
int intDirection(char);
rtBlock * getBlock(int type, char d, int x, int y);

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
    
    rtLevel * m_level;
    
    bool m_draggable;

    void setDraggable(bool d) {
        m_draggable = d;
    }
    
    void setImage(char type='\0', const char * suffix=NULL) {
        char t = (type == '\0' ? m_type : type);
        const char * s = (suffix == NULL ? (std::string("")+charDirection(direction())).c_str() : suffix);
        SDL_Surface * image = rtResource::loadImage("block", (std::string("")+t).c_str(), s);
        if(image == NULL) {
            std::cerr<<"Could not load block "<<type<<" image\n";
            return;
        }
        m_image = image;
    }
    
public:
    enum {ARROW='a', BOMB='b', DEFLECTOR='d', LAUNCHER='l', PRISM='p', SWITCH='s', WALL='w', ENERGISER='x', MULTI_SWITCH='m', CYCLOTRON='c'};
    enum {UP, LEFT, DOWN, RIGHT};
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
    char type() const { return m_type; }
    
    void registerLevel(rtLevel * lvl) {
        m_level = lvl;
    }
    
    rtLevel * level() { return m_level; }
    
    virtual void setX(int x) {
        m_x = x;
    }
    
    virtual void setY(int y) {
        m_y = y;
    }
    
    bool draggable() const { return m_draggable; }
    
    virtual void setDirection(int dir) {
        m_direction = dir;
        setImage();
    }
    
    int direction() const { return m_direction; }
        
    virtual void display(SDL_Surface *surf, int offsetX, int offsetY) const {
        SDL_Rect r;
        r.x = offsetX + m_x;
        r.y = offsetY + m_y;
        SDL_BlitSurface(m_image, NULL, surf, &r);
    }
    
    virtual bool handlePhoton(rtPhoton &photon) {
        return false;
    }
    
    virtual bool handlePhotonEdge(rtPhoton &photon) {
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
        setX(x+WIDTH/2);
        setY(y+HEIGHT/2);
    }
    
    void setHighEnergy(bool he) {
        m_highEnergy = he;
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
    rtLauncher(int dir, int x, int y) : rtBlock(LAUNCHER, dir, x, y) {}
    
    bool clicked() {
        m_level->registerPhoton(new rtPhoton(direction(), x(), y()));
        return true;
    }
};

class rtSwitch : public rtBlock {
    bool m_on;
    
protected:
    void toggleState() {
        m_on = !m_on;
        setImage(m_type, (m_on ? "on" : "off"));        
    }
    
public:
    rtSwitch(int dir, int x, int y) : rtBlock(SWITCH, dir, x, y) {
        m_on = true;
        toggleState();
    }
    bool handlePhoton(rtPhoton &photon) {
        photon.setHighEnergy(false);
        if(!on()) {
            toggleState();
            m_level->switchToggled(this);
        }
        return true;
    }
    
    bool on() { return m_on; }
};

class rtMultiSwitch : public rtSwitch {
public:
    rtMultiSwitch(int dir, int x, int y) : rtSwitch(dir, x, y) {
        m_type = MULTI_SWITCH;
        //toggle state once to undo switch's toggle and then once more to get the right image
        toggleState();
        toggleState();
    }
    
    bool handlePhoton(rtPhoton &photon) {
        photon.setHighEnergy(false);
        //always toggle
        //if(!on()) {
        toggleState();
        m_level->switchToggled(this);
        //}
        return true;
    }
};

class rtCyclotron : public rtBlock {
    int getRotation(int dirToGo, int photonDir) {
        switch(dirToGo) {
            case LEFT:
                switch(photonDir) {
                    case UP: return LEFT;
                    case LEFT: return DOWN;
                    case DOWN: return RIGHT;
                    case RIGHT: return UP;
                }
                break;
                
            case RIGHT:
                switch(photonDir) {
                    case UP: return RIGHT;
                    case LEFT: return UP;
                    case DOWN: return LEFT;
                    case RIGHT: return DOWN;
                }
        }
    }
    
public:
    rtCyclotron(int dir, int x, int y) : rtBlock(CYCLOTRON, dir, x, y) {
        setDraggable(true);
    }
    
    bool handlePhoton(rtPhoton &photon) {
        if(direction() == UP || direction() == LEFT) {
            photon.setDirection(getRotation(LEFT, photon.direction()));
        }
        else {
            photon.setDirection(getRotation(RIGHT, photon.direction()));
        }
    }
};
    
class rtBomb : public rtBlock {
public:
    rtBomb(int dir, int x, int y) : rtBlock(BOMB, dir, x, y) {}
    
    bool handlePhoton(rtPhoton &photon) {
        m_level->gameOver(false);
        return true;
    }
};

class rtWall : public rtBlock {
public:
    rtWall(int dir, int x, int y) : rtBlock(WALL, dir, x, y) {}
    
    bool handlePhotonEdge(rtPhoton &photon) {
        switch(photon.direction()) {
            case UP:photon.setDirection(DOWN); break;
            case DOWN:photon.setDirection(UP); break;
            case LEFT:photon.setDirection(RIGHT); break;
            case RIGHT:photon.setDirection(LEFT); break;
        }
        return true;
    }
};

class rtDeflector : public rtBlock {
public:
    rtDeflector(int dir, int x, int y) : rtBlock(DEFLECTOR, dir, x, y) {
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
    rtPrism(int dir, int x, int y) : rtBlock(PRISM, dir, x, y) {
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
    rtArrow(int dir, int x, int y) : rtBlock(ARROW, dir, x, y) {
        setDraggable(true);
    }
    
    bool handlePhoton(rtPhoton &photon) {
        photon.setDirection(direction());
    }
};

class rtEnergiser : public rtBlock {
public:
    rtEnergiser(int dir, int x, int y) : rtBlock(ENERGISER, dir, x, y) {}
    
    bool handlePhoton(rtPhoton &photon) {
        photon.setHighEnergy(true);
    }
};

#endif

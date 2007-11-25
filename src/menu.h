/******************************
 * highscore.h - manages high scores
 * Independent framework. Can be
 * used in other projects.
 *
 * Created - May 26, 2007
 *
 * (C) Nikhil Marathe <nsm.nikhil@gmail.com>
 * Licensed under the GNU GPL
 */

#ifndef RT_rtMenuItem_H
#define RT__rtMenuItem_H

#include<iostream>
#include<vector>
#include<SDL/SDL.h>

/**
* rtMenuItem is a class implementing a menu item in SDL.
* It requires the SDL and SDL_ttf libraries to be installed.
*
* rtMenuItem can be used independently or through the Menu class.
* rtMenuItem is quite customisable and allows inheritance to add custom behaviour
* such as the use of images or fancy effects.
*
* Most commonly you will just want to use rtMenuItem as is, specifying only the text
* and the callback.
* The callback should be a function with the following prototype
* <code>
* void <function name>(SDL_Event &)
* </code>
*
* If using rtMenuItem on its own you should ensure that your event loop passes
* all events to its handle() method and that you call rtMenuItem's display() method.
*
* Here is an example
* <code>
* //other includes
* #include "menu.h"
*
* void main() {
*     ... initialization code...
*     rtMenuItem item(100, 50, "Click Here", helloWorld);
*     
*     //begin loop
*     while(true) {
*     while(SDL_PollEvent(&event)) {
*         item.handle(event);
*         item2.handle(event);
*        
*     }
*     item.display(screen);
*     ... more code...
* }
* </code>
*
* To customise the Look 'N' Feel of the rtMenuItem reimplement the
* displayNormal() and displayHover() methods.
* Draw on the SDL_Surface surf which is a class member.
*/         
class rtMenuItem {
private:

    enum State { NORMAL, HOVER};

    State currentState;
    
    std::string itemText;
    void (*actionCallback)(SDL_Event &);

    //colours
    SDL_Color foregroundNormal, backgroundNormal, borderNormal, foregroundHover, backgroundHover, borderHover;

    int width, height, borderWidth;

    //X and Y offsets set by Constructor 
    //display surface, set by display before calling other display functions
    int oX, oY;
    SDL_Surface * surf;

    Uint32 getMappedColor(SDL_Color);

    SDL_Rect getBackgroundRect();

protected:
    /**
    * Draws the border around the rtMenuItem
    */
    void drawBorder();
    /**
    * Draws the text
    */
    void drawText();
    /**
    * Draws the menu item in the 'normal' state when the mouse is not over it.
    */
    void displayNormal();
    /**
    * Draws the menu item in the 'hover' state when the mouse is over it.
    */
    void displayHover();

public:
    rtMenuItem(int x, int y, char * , void (*callback)(SDL_Event &));

    static SDL_Color createColor(int r, int g, int b);
    static SDL_Rect createRect(int, int, int, int);

    void setText(std::string text);
    void setAction(void (*callback)(SDL_Event &));
    void setDimensions(int, int);

    void display(SDL_Surface *);

    void handle(SDL_Event&);

    bool pointInsideThis(int, int);

    //setters
    void setBackground(SDL_Color col) {
        backgroundNormal = col;
    };

    void setForeground(SDL_Color col) {
        foregroundNormal = col;
    };

    void setBorder(SDL_Color col) {
        borderNormal = col;
    };

    void setBackgroundHover(SDL_Color col) {
        backgroundHover = col;
    };

    void setForegroundHover(SDL_Color col) {
        foregroundHover = col;
    };

    void setBorderHover(SDL_Color col) {
        borderHover = col;
    };

    void setBorderWidth(int width) {
        borderWidth = width;
    };

    void setX(int x) {
        oX = x;
    };

    void setY(int y) {
        oY = y;
    };

    void setWidth(int w) {
        width = w;
    };

    void setHeight(int h) {
        height = h;
    };

    int getWidth() {
        return width;
    };

    int getHeight() {
        return height;
    };

};


class Menu {
private:
    vector<rtMenuItem> rtMenuItems;
    int x, y;

    int paddingY;

public:
    Menu(int X, int Y) {
        x = X;
        y = Y;
        paddingY = 10;
    };

    Menu() {
        x = 0;
        y = 0;
        paddingY = 10;
    };

    void setX(int X) {
        x = X;
    };

    void setY(int Y) {
        y = Y;
    };

    void addItem(char * text, void (*callback)(SDL_Event &)) {
        addItem(rtMenuItem(0, 0, text, callback));
    };

    void addItem(rtMenuItem m) {
        m.setX(x);
        rtMenuItems.push_back(m);
    };

    void display(SDL_Surface * surf) {
        int dispY = y;
        for(unsigned int i = 0; i < rtMenuItems.size(); ++i) {
            rtMenuItems[i].setY(dispY);
            rtMenuItems[i].display(surf);

            dispY += rtMenuItems[i].getHeight() + paddingY;
        }
    };

    void handle(SDL_Event & event) {
        for(unsigned int i = 0; i < rtMenuItems.size(); ++i) {
            rtMenuItems[i].handle(event);
        }
    };
};

#endif

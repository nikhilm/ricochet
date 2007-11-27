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

#ifndef RT_MENU_H
#define RT_MENU_H

#include<iostream>
#include<vector>
#include<SDL/SDL.h>

/**
 * Menu action class
 * rtMenuItem accepts this as a click action
 * You should subclass and create specific situation handlers
 * by overloading trigger
 */
class rtMenuAction {
public:
    virtual void trigger(const SDL_Event&) {};
};

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
    
    rtMenuAction * action;

    //colours
    SDL_Color foregroundNormal, backgroundNormal, borderNormal, foregroundHover, backgroundHover, borderHover;

    int width, height, borderWidth;

    //X and Y offsets set by Constructor 
    //display surface, set by display before calling other display functions
    int oX, oY;
    SDL_Surface * surf;

    SDL_Rect getBackgroundRect() {
        return createRect(oX + borderWidth, oY + borderWidth, width - 2*borderWidth, height - 2*borderWidth);
    }

    Uint32 getMappedColor(SDL_Color col) {
        return SDL_MapRGB(surf->format, col.r, col.g, col.b);
    }
protected:
    /**
     * Draws the border around the rtMenuItem
     */
    virtual void drawBorder() {
        SDL_FillRect(surf, &createRect(oX, oY, width, height), getMappedColor((currentState == NORMAL ? borderNormal : borderHover)));
    //fill in the remaining with black
        SDL_FillRect(surf, &getBackgroundRect(), getMappedColor(createColor(0, 0, 0)));

    }

    /**
     * Draws the menu item in the 'normal' state when the mouse is not over it.
     */
    virtual void displayNormal() {
        drawBorder();
        SDL_FillRect(surf, &getBackgroundRect(), getMappedColor(backgroundNormal));
        drawText();
    }
    /**
     * Draws the menu item in the 'hover' state when the mouse is over it.
    */
    virtual void displayHover() {
        drawBorder();
        SDL_FillRect(surf, &getBackgroundRect(), getMappedColor(backgroundHover));
        drawText();
    }

    
    virtual void drawText() {
    //calculate the centre
        rtTextUtil::render(itemText.c_str(), (currentState == HOVER ? foregroundHover : foregroundNormal), RT_MEDIUM_FONT, surf, oX + width/2, oY + height/2, rtTextUtil::ALIGN_CENTER, (currentState == HOVER ? true : false)); //last argument is bold
    }
public:
//     rtMenuItem(int, int, const std::string&, rtMenuAction *);
// 
//     static SDL_Color createColor(int r, int g, int b);
//     static SDL_Rect createRect(int, int, int, int);
//     
//     void setAction(rtMenuAction*);
//     void setDimensions(int, int);
// 
//     void display(SDL_Surface *screen);
// 
//     void handle(SDL_Event &event);
// 
//     bool pointInsideThis(int pX, int pY);

    //setters    
    void setText(const std::string text) {
        itemText = text;
    }
    
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

    rtMenuItem(int x, int y, const std::string& text, rtMenuAction * callback) {
        itemText = text;
        action = callback;
        currentState = NORMAL;

        setX(x);
        setY(y);
        setDimensions(300, 75);
        setBorderWidth(5);

        //default white and black theme
        SDL_Color white = createColor(255, 255, 255);
        SDL_Color black = createColor(0, 0, 0);
        SDL_Color red = createColor(255, 0, 0);
        setForeground(red);
        setBackground(black);
        setBorder(black);

        setForegroundHover(red);
        setBackgroundHover(black);
        setBorderHover(black);


    }

    SDL_Color createColor(int r, int g, int b) {
        SDL_Color col;
        col.r = r;
        col.g = g;
        col.b = b;
        return col;
    }

    SDL_Rect createRect(int x, int y, int w, int h) {
        SDL_Rect r;
        r.x = x;
        r.y = y;
        r.w = w;
        r.h = h;
        return r;
    }

    void setAction(rtMenuAction * callback) {
        action = callback;
    }

    void setDimensions(int w, int h) {
        width = w;
        height = h;
    }

    void display(SDL_Surface *screen) {
        surf = screen;

        if(currentState == NORMAL)
            displayNormal();
        else if(currentState == HOVER)
            displayHover();
    }

    bool pointInsideThis(int pX, int pY) {
        return ( pX >= oX && pX <= (oX + width) && pY >= oY && pY <= (oY + height) );
    }

    void handle(SDL_Event &event) {
        if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT
           && pointInsideThis(event.button.x, event.button.y)) {
            action->trigger(event);
           }
           else if(event.type == SDL_MOUSEMOTION) {
               if(pointInsideThis(event.motion.x, event.motion.y)) {
                   currentState = HOVER;
               }
               else {
                   currentState = NORMAL;
               }
           }
    }
};


class rtMenu {
private:
    std::vector<rtMenuItem> rtMenuItems;
    int x, y;

    int paddingY;

public:
    rtMenu(int X, int Y) {
        x = X;
        y = Y;
        paddingY = 10;
    };

    rtMenu() {
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

    void addItem(const std::string &text, rtMenuAction * callback) {
        rtMenuItem item = rtMenuItem(0, 0, text, callback);
        addItem(item);
    };

    void addItem(rtMenuItem& m) {
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

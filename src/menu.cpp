/******************************
 * menu.h - a menu framework as long as textutil.h is also available
 * Independent framework. Can be
 * used in other projects.
 *
 * Created - May 26, 2007
 *
 * (C) Nikhil Marathe <nsm.nikhil@gmail.com>
 * Licensed under the GNU GPL
 */

#include "menu.h"
#include "textutil.h"

    
rtMenuItem::rtMenuItem(int x, int y, char * text, void (*callback)(SDL_Event &)) {
    itemText = text;
    actionCallback = callback;
    currentState = NORMAL;

    setX(x);
    setY(y);
    setDimensions(300, 75);
    setBorderWidth(5);

    //default white and black theme
    SDL_Color white = createColor(255, 255, 255);
    SDL_Color black = createColor(0, 0, 0);
    SDL_Color red = createColor(255, 0, 0);
    setForeground(white);
    setBackground(black);
    setBorder(white);

    setForegroundHover(red);
    setBackgroundHover(black);
    setBorderHover(red);


}

SDL_Color rtMenuItem::createColor(int r, int g, int b) {
    SDL_Color col;
    col.r = r;
    col.g = g;
    col.b = b;
    return col;
}

SDL_Rect rtMenuItem::createRect(int x, int y, int w, int h) {
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    return r;
}

SDL_Rect rtMenuItem::getBackgroundRect() {
    return createRect(oX + borderWidth, oY + borderWidth, width - 2*borderWidth, height - 2*borderWidth);
}

Uint32 rtMenuItem::getMappedColor(SDL_Color col) {
    return SDL_MapRGB(surf->format, col.r, col.g, col.b);
}

TTF_Font * rtMenuItem::loadFont(char * file, int size) {
    //try loading SDL_ttf
    if(!TTF_WasInit() && TTF_Init() == -1) {
        cerr<<"Error initializing SDL_ttf library, cannot draw text. Error: "<<TTF_GetError()<<endl;
    }

    TTF_Font *f = TTF_OpenFont(file, size);
    if(!f) {
        cout<<"Error loading font file"<<TTF_GetError()<<"\n";
    }
    return f;
}

void rtMenuItem::setAction(const rtMenuAction& callback) {
    actionaction = callback;
}

void rtMenuItem::setDimensions(int w, int h) {
    width = w;
    height = h;
}

void rtMenuItem::display(SDL_Surface *screen) {
    surf = screen;

    if(currentState == NORMAL)
        displayNormal();
    else if(currentState == HOVER)
        displayHover();
}

bool rtMenuItem::pointInsideThis(int pX, int pY) {
    return ( pX >= oX && pX <= (oX + width) && pY >= oY && pY <= (oY + height) );
}

void rtMenuItem::handle(SDL_Event &event) {
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

void rtMenuItem::drawBorder() {
    SDL_FillRect(surf, &createRect(oX, oY, width, height), getMappedColor((currentState == NORMAL ? borderNormal : borderHover)));
    //fill in the remaining with black
    SDL_FillRect(surf, &getBackgroundRect(), getMappedColor(createColor(0, 0, 0)));

}

void rtMenuItem::displayNormal() {
    drawBorder();
    SDL_FillRect(surf, &getBackgroundRect(), getMappedColor(backgroundNormal));
    drawText();
}

void rtMenuItem::displayHover() {
    drawBorder();
    SDL_FillRect(surf, &getBackgroundRect(), getMappedColor(backgroundHover));
    drawText();
}

void rtMenuItem::drawText() {
    //calculate the centre
    rtTextUtil::render(itemText.c_str(), red, RT_MEDIUM_FONT, surf, oX + width/2, oY + height/2, rtTextUtil::ALIGN_CENTER, (currentState == HOVER ? true : false)); //last argument is bold
}


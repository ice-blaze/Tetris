#include "SDL/SDL.h"
#include <string>

#include "Classe_BitmapFont.h"

using namespace std;

#ifndef BUTTON_HPP
#define BUTTON_HPP

class Button : BitmapFont
{
    private:
        int x,y,w,h;
        int selectZoneX,selectZoneY,selectZoneW,selectZoneH;
        bool blSelected,blDoAction;
        SDL_Surface *imageNotSelected;
        SDL_Surface *imageSelected;
        string text;

    public:
        Button(string filenameNotSelected,string filenameSelected,string _text,int _x,int _y,
               int _selectZoneX,int _selectZoneY,int _selectZoneW,int _selectZoneH);
        ~Button();
        void applySurface(SDL_Surface *screen);
        void check(SDL_Event event);
        bool isItSelected(){return blSelected;};
        bool doTheAction(){return blDoAction;};

        int getX(){return x;};
        int getY(){return y;};
        int getW(){return w;};
        int getH(){return h;};

        void setX(int _x){x=_x;};
        void setY(int _y){y=_y;};
        void setW(int _w){w=_w;};
        void setH(int _h){h=_h;};
};

#endif

#include "classic_function.hpp"

#include "Button.hpp"

Button::Button(string filenameNotSelected,string filenameSelected,string _text,int _x,int _y,
               int _selectZoneX,int _selectZoneY,int _selectZoneW,int _selectZoneH):BitmapFont( load_image( "Fonts/font.png" ))
{
    blSelected=false;
    blDoAction=false;
    imageNotSelected=load_image(filenameNotSelected);
    imageSelected=load_image(filenameSelected);

    text=_text;

    x=_x;
    y=_y;
    w=imageNotSelected->w;
    h=imageNotSelected->h;

    selectZoneX=_selectZoneX;
    selectZoneY=_selectZoneY;
    selectZoneW=_selectZoneW;
    selectZoneH=_selectZoneH;
}

Button::~Button()
{
    SDL_FreeSurface(imageNotSelected);
    SDL_FreeSurface(imageSelected);
    fontDelete();
}

void Button::applySurface(SDL_Surface *screen)
{
    if(blSelected==false)
    {
        apply_surface(x,y,imageNotSelected,screen,NULL);
    }
    else
    {
        apply_surface(x,y,imageSelected,screen,NULL);
    }
    showTextFullCenter(text,screen,selectZoneX,selectZoneY,selectZoneW,selectZoneH);
}

void Button::check(SDL_Event event)
{
        switch( event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {


                    //Sinon warning
                    default :
                        break;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if(event.button.button==SDL_BUTTON_LEFT)
                {
                    if(collision(selectZoneX, selectZoneY, selectZoneW, selectZoneH,event.motion.x, event.motion.y,0,0))
                    {
                        blDoAction=true;
                    }
                }
                break;
        }


    if(collision(selectZoneX, selectZoneY, selectZoneW, selectZoneH,event.motion.x, event.motion.y,0,0))
    {
        blSelected=true;
    }
    else
    {
        blSelected=false;
    }
}

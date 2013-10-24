#include "sdl.h"
#include "classic_function.hpp"
#include "Classe_BitmapFont.h"
#include <sstream>

#ifndef HUD_GAME_H
#define HUD_GAME_H

class HUD_Game//:BitmapFont
{
    private:

        SDL_Surface *borderHori,*borderVerti;
        int x,y;


    protected:
        //int iScore;


    public:
        SDL_Surface *surfPiece1,*surfPiece2,*surfPiece3,*surfPiece4,*surfPiece5,*surfPiece6,*surfPiece7;
        int iScore;

        HUD_Game();
        ~HUD_Game();
        //void HUDSetScreen(SDL_Surface *screen);
        void HUDShowPieces(SDL_Surface *screen);
        void HUDShowScore(SDL_Surface *screen);
        int GAME_SCREEN_WIDTH ;
};

#endif

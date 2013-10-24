#include "HUD_Game.hpp"
#include "classic_function.hpp"
#include "Classe_Piece.h"


HUD_Game::HUD_Game()//:BitmapFont(load_image("fonts/font.png"))
{
    iScore=0;

    x=INTERF_MAIN_PIECE_X;
    y=INTERF_MAIN_PIECE_Y;

    surfPiece1 = load_image("Sprites/form1.png");
    surfPiece2 = load_image("Sprites/form2.png");
    surfPiece3 = load_image("Sprites/form3.png");
    surfPiece4 = load_image("Sprites/form4.png");
    surfPiece5 = load_image("Sprites/form5.png");
    surfPiece6 = load_image("Sprites/form6.png");
    surfPiece7 = load_image("Sprites/form7.png");

    borderHori = SDL_CreateRGBSurface(0,INTERF_WIDTH,INTERF_WIDTH_BORDER,32,0,0,0,0);
    SDL_FillRect(borderHori, NULL, SDL_MapRGB(borderHori->format, 128,128,128));
    borderVerti = SDL_CreateRGBSurface(0,INTERF_WIDTH_BORDER,INTERF_WIDTH,32,128,128,128,0);
    SDL_FillRect(borderVerti, NULL, SDL_MapRGB(borderVerti->format, 128,128,128));

    if( surfPiece1 == NULL||
		surfPiece2 == NULL||
		surfPiece3 == NULL||
		surfPiece4 == NULL||
		surfPiece5 == NULL||
		surfPiece6 == NULL||
		surfPiece7 == NULL)
	{
		printf("chargement des image ERROR");
	}
}

HUD_Game::~HUD_Game()
{
    //je ne sais pas pk mais il plante quand il les free...
//	SDL_FreeSurface(surfPiece1);
//	SDL_FreeSurface(surfPiece2);
//	SDL_FreeSurface(surfPiece3);
//	SDL_FreeSurface(surfPiece4);
//	SDL_FreeSurface(surfPiece5);
//	SDL_FreeSurface(surfPiece6);
//	SDL_FreeSurface(surfPiece7);
}

void HUD_Game::HUDShowPieces(SDL_Surface *screen)
{
    if(Piece::nextForme==1)
    {
        apply_surface(x,y-SPRITE_WIDTH,surfPiece1,screen,NULL);
        apply_surface(x,y,surfPiece1,screen,NULL);
        apply_surface(x,y+SPRITE_WIDTH,surfPiece1,screen,NULL);
        apply_surface(x,y+SPRITE_WIDTH*2,surfPiece1,screen,NULL);
    }
    else if(Piece::nextForme==2)
    {
        int x2=x-SPRITE_WIDTH/2,y2=y-SPRITE_WIDTH/2;
        apply_surface(x2,y2,surfPiece2,screen,NULL);
        apply_surface(x2,y2+SPRITE_WIDTH,surfPiece2,screen,NULL);
        apply_surface(x2,y2+SPRITE_WIDTH*2,surfPiece2,screen,NULL);
        apply_surface(x2+SPRITE_WIDTH,y2+SPRITE_WIDTH*2,surfPiece2,screen,NULL);
    }
    else if(Piece::nextForme==3)
    {
        int x2=x+SPRITE_WIDTH/2,y2=y-SPRITE_WIDTH/2;
        apply_surface(x2,y2,surfPiece3,screen,NULL);
        apply_surface(x2,y2+SPRITE_WIDTH,surfPiece3,screen,NULL);
        apply_surface(x2,y2+SPRITE_WIDTH*2,surfPiece3,screen,NULL);
        apply_surface(x2-SPRITE_WIDTH,y2+SPRITE_WIDTH*2,surfPiece3,screen,NULL);
    }
    else if(Piece::nextForme==4)
    {
        apply_surface(x,y,surfPiece4,screen,NULL);
        apply_surface(x+SPRITE_WIDTH,y,surfPiece4,screen,NULL);
        apply_surface(x-SPRITE_WIDTH,y,surfPiece4,screen,NULL);
        apply_surface(x,y+SPRITE_WIDTH,surfPiece4,screen,NULL);

    }
    else if(Piece::nextForme==5)
    {
        //+1 à cause de la divisions qui n'arondie pas bien
        apply_surface(x-SPRITE_WIDTH/2,y,surfPiece5,screen,NULL);
        apply_surface(x+SPRITE_WIDTH/2+1,y,surfPiece5,screen,NULL);
        apply_surface(x-SPRITE_WIDTH/2,y+SPRITE_WIDTH,surfPiece5,screen,NULL);
        apply_surface(x+SPRITE_WIDTH/2+1,y+SPRITE_WIDTH,surfPiece5,screen,NULL);
    }
    else if(Piece::nextForme==6)
    {
        apply_surface(x,y,surfPiece6,screen,NULL);
        apply_surface(x-SPRITE_WIDTH,y,surfPiece6,screen,NULL);
        apply_surface(x,y+SPRITE_WIDTH,surfPiece6,screen,NULL);
        apply_surface(x+SPRITE_WIDTH,y+SPRITE_WIDTH,surfPiece6,screen,NULL);
    }
    else if(Piece::nextForme==7)
    {
        apply_surface(x,y,surfPiece7,screen,NULL);
        apply_surface(x-SPRITE_WIDTH,y,surfPiece7,screen,NULL);
        apply_surface(x,y+SPRITE_WIDTH,surfPiece7,screen,NULL);
        apply_surface(x+SPRITE_WIDTH,y+SPRITE_WIDTH,surfPiece7,screen,NULL);
    }
}

void HUD_Game::HUDShowScore(SDL_Surface *screen)
{
    BitmapFont font(load_image("fonts/font.png"));
    std::ostringstream out;
    out <<"Score : \n"<< iScore;
    font.show_text( INTERF_X, 0, out.str(), screen );
    apply_surface(INTERF_X,INTERF_Y+INTERF_HEIGHT-INTERF_WIDTH_BORDER,borderHori,screen,NULL);
    apply_surface(INTERF_X,INTERF_Y ,borderHori,screen,NULL);
    apply_surface(INTERF_X,INTERF_Y ,borderVerti,screen,NULL);
    apply_surface(INTERF_X+INTERF_WIDTH-INTERF_WIDTH_BORDER,INTERF_Y ,borderVerti,screen,NULL);
    font.fontDelete();
}

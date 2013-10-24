#include "Menu_Option.hpp"
#include "Constantes.h"

Menu_Option::Menu_Option()
{

}

int Menu_Option::Start()
{
    int retVal=0;//0=menu;1=jeu;2=menu option;-1=arret du prog
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Mise en place de l'écran
    SDL_WM_SetCaption( "Tetris", NULL );
    SDL_WM_SetIcon(SDL_LoadBMP( "sprites/ico.bmp" ),NULL);
    SDL_EnableKeyRepeat(350,100);
    putenv("SDL_VIDEO_WINDOW_POS=center"); //pour centrer la fenêtre
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //S'il y a eu une erreur lors de la mise en place de l'écran
    if( screen == NULL )
    {
        return false;
    }

	//Initialisation de SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }


    int quit = false;
	while( quit == false )
    {
        //Tant qu'il y a un événement
        while( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym)
				{
//
//					case SDLK_DOWN:
//						iForward=5;
//						break;

                    //Sinon warning
                    default :
                        retVal=0;
                        quit=true;
                        break;
				}
			}

            //Si l'utilisateur a cliqué sur le X de la fenêtre
            if( event.type == SDL_QUIT )
            {
                //On quitte the programme
                quit = true;
                retVal=-1;
            }


        }

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 16,19,58));

        if( SDL_Flip( screen ) == -1 )
        {
            printf("\nn'arrive pas à flipper\n");
            return -1;
        }
    }

    SDL_FreeSurface(screen);
	Mix_CloseAudio();
	SDL_Quit();

    return retVal;
}

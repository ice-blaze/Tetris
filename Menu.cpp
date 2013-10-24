#include "Menu.hpp"
#include "Menu_Option.hpp"
#include "classic_function.hpp"
#include <SDL/SDL_getenv.h>
#include <SDL_mixer.h>

Menu::Menu()
{
    screen = NULL;
    music = NULL;
}

int Menu::Start()
{
    //0 le jeu se lance 1 le menu arrête tout
    int retVal = 0;//0=menu;1=jeu;2=menu option;-1=arret du prog

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

    //Credit
    BitmapFont font=load_image("fonts/font.png");
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 16,19,58));
    //font.show_text_fullCenter("ababapapapa",screen,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    font.showTextFullCenter("Credit:"
                              "\nSprites: Onimanta"
                              "\n& Ice-Balze"
                              "\nProgramming: Ice-Blaze"
                              "\n\nClick or press a key !",screen,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    SDL_Flip(screen);
    waitInput(event);
    font.fontDelete();
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    //fin crédit


    //Mise en place des bouttons
    int btnPlayX=SCREEN_WIDTH-308,btnPlayY=0;
    int btnOptionX=0,btnOptionY=GAME_SCREEN_HEIGHT-170;
    int btnQuitX=SCREEN_WIDTH-184,btnQuitY=SCREEN_HEIGHT-106;
    Button btnPlay("Sprites/menu/btn_Play_G.png","Sprites/menu/btn_Play_R.png","Play",btnPlayX,btnPlayY,btnPlayX+23,btnPlayY+100,252,85);
    Button btnOption("Sprites/menu/btn_option_g.png","Sprites/menu/btn_option_r.png","Option",btnOptionX,btnOptionY,btnOptionX+40,btnOptionY,154,63);
    Button btnQuit("Sprites/menu/btn_quit_g.png","Sprites/menu/btn_quit_r.png","Quit",btnQuitX,btnQuitY,btnQuitX+10,btnQuitY,150,60);

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

                        //quit=true;
                        break;
				}
			}

            //Si l'utilisateur a cliqué sur le X de la fenêtre
            if( event.type == SDL_QUIT )
            {
                //On quitte the programme
                retVal=-1;
                quit = true;
            }


        }

        btnPlay.check(event);
        if(btnPlay.doTheAction())
        {
            quit=true;
            retVal=1;//lance le tetris
            loadCFG();
        }

        btnOption.check(event);
        if(btnOption.doTheAction())
        {
           quit=true;
           retVal=2;//option menu sera lancé
        }

        btnQuit.check(event);
        if(btnQuit.doTheAction())
        {
            quit=true;
            retVal=-1;//ne lance pas le tetris après
        }


        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 16,19,58));
        btnPlay.applySurface(screen);
        btnOption.applySurface(screen);
        btnQuit.applySurface(screen);

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

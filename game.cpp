#include "game.hpp"

void Game::refreshVariable()
{
//    for (int i=0;i<GAME_SCREEN_WIDTH/SPRITE_WIDTH;i++)
//    {
//    	for (int u=0;u<GAME_SCREEN_HEIGHT/SPRITE_WIDTH;u++)
//    	{
//    		tab[i][u]=0;
//    	}
//    }

    iChute = -1;
    iGameSpeed = 10;
    iForward = 60;
//    HUD.iScore = 0;
    iPrevSens = NULL;
    iPrevPos = NULL;
    iGOSpeed = 100;


//    GRID_WIDTH = 30;
//  	GRID_HEIGHT = 20;
}

Game::Game()//:HUD_Game()
{
    //Les surfaces
    screen=NULL;
    surf_form0=NULL;
    surf_form1=NULL;
    surf_form2=NULL;
    surf_form3=NULL;
    surf_form4=NULL;
    surf_form5=NULL;
    surf_form6=NULL;
    surf_form7=NULL;
    surf_gameOver=NULL;
    surfFont = NULL;

    music = NULL;
    //La structure d'événements que nous allons utiliser

    //grille
    refreshVariable();
    //tableau de pièce
}
//
//void clean_up();
//bool load_files();
//bool init();
//void dieDisplay();
//void affichage();
//bool checkTetris();
//
//void affichage();
//bool addpintab();
//bool delpintab();
//void dieDisplay();

bool Game::init()
{
    //création de la grille
    for ( int i = 0; i < GAME_SCREEN_WIDTH/SPRITE_WIDTH; i++ )
    {
        tab.push_back ( vector<char>() );
        for ( int j = 0; j < GAME_SCREEN_HEIGHT/SPRITE_WIDTH; j++ )
            tab[i].push_back (0);
    }

    //tab = new char[GAME_SCREEN_WIDTH/SPRITE_WIDTH][GAME_SCREEN_HEIGHT/SPRITE_WIDTH];
    refreshVariable();

    //Initialisation de tous les sous-système de SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Mise en place de l'écran
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

	SDL_WM_SetIcon(load_image("sprites/ico.png"),NULL);
	SDL_EnableKeyRepeat(350,100);

    //Mise en place de la barre caption
    SDL_WM_SetCaption( "Tetris", NULL );

	//Initialise la grille
//	for (int i = 0; i < GAME_SCREEN_HEIGHT/SPRITE_WIDTH;i++)
//	{
//		for (int u = 0; u < GAME_SCREEN_WIDTH/SPRITE_WIDTH;u++)
//		{
//			//tab[i][u]=0;
//		}
//	}

    //Si tout c'est bien passé
    return true;
}

bool Game::load_files()
{
    //Chargement du point
	surf_form0 = load_image( "sprites/form0.png" );
	surf_form1 = load_image( "sprites/form1.png" );
	surf_form2 = load_image( "sprites/form2.png" );
	surf_form3 = load_image( "sprites/form3.png" );
	surf_form4 = load_image( "sprites/form4.png" );
	surf_form5 = load_image( "sprites/form5.png" );
	surf_form6 = load_image( "sprites/form6.png" );
	surf_form7 = load_image( "sprites/form7.png" );
	surf_gameOver = load_image("sprites/gameover.png");
	surfFont = load_image( "Fonts/font.png" );



    //S'il y a eu un problème au chargement du point
    if( surf_form0 == NULL||
		surf_form1 == NULL||
		surf_form2 == NULL||
		surf_form3 == NULL||
		surf_form4 == NULL||
		surf_form5 == NULL||
		surf_form6 == NULL||
		surf_form7 == NULL||
		surfFont == NULL||
		surf_gameOver == NULL)
	{
		printf("chargement des image ERROR");
        return false;
	}

	//Chargement de la musique
    //music = Mix_LoadMUS( "Sounds/8bitDiego.mp3" );

    //S'il y a eu une erreur au chargement de la musique
    //if( music == NULL )return false;

    //Si tout s'est bien passé
    return true;
}

void Game::clean_up()
{
	SDL_FreeSurface(screen);
    //Libèration des surfaces
	SDL_FreeSurface(surf_form0);
	SDL_FreeSurface(surf_form1);
	SDL_FreeSurface(surf_form2);
	SDL_FreeSurface(surf_form3);
	SDL_FreeSurface(surf_form4);
	SDL_FreeSurface(surf_form5);
	SDL_FreeSurface(surf_form6);
	SDL_FreeSurface(surf_form7);

	//création de la grille
    for ( int i = GAME_SCREEN_WIDTH/SPRITE_WIDTH; i > 0; i-- )
    {
        for ( int j = GAME_SCREEN_HEIGHT/SPRITE_WIDTH; j > 0; j-- )
            tab[i].pop_back ();
        tab.pop_back ();
    }

	//Libération de la musique
    //Mix_FreeMusic( music );

	//On quitte SDL_mixer
    Mix_CloseAudio();
    //On quitte SDL
    SDL_Quit();
}

int Game::Start()
{
    int retVal=0;//0=menu;1=jeu;2=menu option;-1=arret du prog
    //Ce qui va nous permettr de quitter
    bool quit = false;

    //Le regulateur
    Timer fps;

    //Initialisation
    if( init() == false )
    {
		printf("initialisation du prog ERROR");
        return 1;
    }

    //Interface
    BitmapFont font( surfFont );
    HUD_Game HUD;


	for (int i = 0; i < GAME_SCREEN_HEIGHT/SPRITE_WIDTH;i++)
	{
		printf("\n");
		for (int u = 0; u < GAME_SCREEN_WIDTH/SPRITE_WIDTH;u++)
		{
			printf("%d",tab[u][i]);
		}
	}

    //Chargement des fichiers
    if( load_files() == false )
    {
		printf("chargement des fichhiers ERROR");
        return 1;
    }

	//On lance la musique
    //if( Mix_PlayMusic( music, -1 ) == -1 )
    //{
        //return 1;
    //}
	//On demarre le timer fps
        fps.start();

    //Tant que l'utilisateur n'a pas quitter
    while( quit == false )
    {

        //Tant qu'il y a un événement
        while( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_KEYDOWN && iChute!=-1)
			{
				switch(event.key.keysym.sym)
				{
				    case SDLK_ESCAPE:
                        quit = true;
                        retVal=-1;
                        break;

					case SDLK_UP:
						if(//pièce 1
							(tabP[iChute].forme==1&&tabP[iChute].x>0&&tabP[iChute].x+2<GAME_SCREEN_WIDTH/SPRITE_WIDTH&&tabP[iChute].y+3<GAME_SCREEN_HEIGHT/SPRITE_WIDTH)||
							//pièce 2
							(tabP[iChute].forme==2&&tabP[iChute].sens==0&&tabP[iChute].x>0)||
							(tabP[iChute].forme==2&&tabP[iChute].sens==2&&tabP[iChute].x+1<GAME_SCREEN_WIDTH/SPRITE_WIDTH)||
							(tabP[iChute].forme==2&&tabP[iChute].sens==3&&tabP[iChute].y+1<GAME_SCREEN_HEIGHT/SPRITE_WIDTH)||
							//pièce 3
							(tabP[iChute].forme==3&&tabP[iChute].sens==0&&tabP[iChute].x+1<GAME_SCREEN_WIDTH/SPRITE_WIDTH)||
							(tabP[iChute].forme==3&&tabP[iChute].sens==2&&tabP[iChute].x>0)||
							(tabP[iChute].forme==3&&tabP[iChute].sens==1&&tabP[iChute].y+1<GAME_SCREEN_HEIGHT/SPRITE_WIDTH)||
							//pièce 4
							(tabP[iChute].forme==4&&tabP[iChute].sens==1&&tabP[iChute].x+1<GAME_SCREEN_WIDTH/SPRITE_WIDTH)||
							(tabP[iChute].forme==4&&tabP[iChute].sens==2&&tabP[iChute].y+1<GAME_SCREEN_HEIGHT/SPRITE_WIDTH)||
							(tabP[iChute].forme==4&&tabP[iChute].sens==3&&tabP[iChute].x>0)||
							//pièce 6
							(tabP[iChute].forme==6&&tabP[iChute].sens==1&&tabP[iChute].x+1<GAME_SCREEN_WIDTH/SPRITE_WIDTH)||
							//pièce 7
							(tabP[iChute].forme==7&&tabP[iChute].sens==1&&tabP[iChute].x+1<GAME_SCREEN_WIDTH/SPRITE_WIDTH)||

							//cas ou pas besoin de condition mais besoin de les mettres dans les if si non ils ne sont pas gérés
							//pour être tournées
							(tabP[iChute].forme==2&&tabP[iChute].sens==1)||//pièce 2
							(tabP[iChute].forme==3&&tabP[iChute].sens==3)||//pièce 3
							(tabP[iChute].forme==4&&tabP[iChute].sens==0)||//pièce 4
							(tabP[iChute].forme==6&&tabP[iChute].sens==0)||
							(tabP[iChute].forme==7&&tabP[iChute].sens==0))
						{
							iPrevSens=tabP[iChute].sens;
							delpintab();
							tabP[iChute].changeSens();
							if(addpintab()==false)
							{
								tabP[iChute].sens=iPrevSens;
								if(addpintab()==false)
								{
									printf("plantage changement de sens");
									quit=true;
								}
							}
						}
						break;

					case SDLK_RIGHT:
						//piece1
						if((tabP[iChute].x+1<GAME_SCREEN_WIDTH/SPRITE_WIDTH&&tabP[iChute].forme==1&&tabP[iChute].sens==0)||
							(tabP[iChute].x+3<GAME_SCREEN_WIDTH/SPRITE_WIDTH&&tabP[iChute].forme==1&&tabP[iChute].sens==1)||
							//piece2
							(tabP[iChute].x+2<GAME_SCREEN_WIDTH/SPRITE_WIDTH&&(tabP[iChute].forme==2&&tabP[iChute].sens==0))||
							(tabP[iChute].x+2<GAME_SCREEN_WIDTH/SPRITE_WIDTH&&(tabP[iChute].forme==2&&tabP[iChute].sens==1))||
							(tabP[iChute].x+1<GAME_SCREEN_WIDTH/SPRITE_WIDTH&&(tabP[iChute].forme==2&&tabP[iChute].sens==2))||
							(tabP[iChute].x+2<GAME_SCREEN_WIDTH/SPRITE_WIDTH&&(tabP[iChute].forme==2&&tabP[iChute].sens==3))||
							//pièce 3
							(tabP[iChute].x+1<GAME_SCREEN_WIDTH/SPRITE_WIDTH&&(tabP[iChute].forme==3&&tabP[iChute].sens==0))||
							(tabP[iChute].x+2<GAME_SCREEN_WIDTH/SPRITE_WIDTH&&(tabP[iChute].forme==3&&tabP[iChute].sens==1))||
							(tabP[iChute].x+2<GAME_SCREEN_WIDTH/SPRITE_WIDTH&&(tabP[iChute].forme==3&&tabP[iChute].sens==2))||
							(tabP[iChute].x+2<GAME_SCREEN_WIDTH/SPRITE_WIDTH&&(tabP[iChute].forme==3&&tabP[iChute].sens==3))||
							//pièce 4
							(tabP[iChute].x+2<GAME_SCREEN_WIDTH/SPRITE_WIDTH&&tabP[iChute].forme==4&&(tabP[iChute].sens==0||tabP[iChute].sens==2||tabP[iChute].sens==3))||
							(tabP[iChute].x+1<GAME_SCREEN_WIDTH/SPRITE_WIDTH&&tabP[iChute].forme==4&&tabP[iChute].sens==1)||
							//pièce 5
							(tabP[iChute].x+2!=GAME_SCREEN_WIDTH/SPRITE_WIDTH&&tabP[iChute].forme==5)||
							//pièce 6
							(tabP[iChute].x+1!=GAME_SCREEN_WIDTH/SPRITE_WIDTH&&tabP[iChute].forme==6&&tabP[iChute].sens==1)||
							(tabP[iChute].x+2!=GAME_SCREEN_WIDTH/SPRITE_WIDTH&&tabP[iChute].forme==6&&tabP[iChute].sens==0)||
							//pièce 7
							(tabP[iChute].x+2!=GAME_SCREEN_WIDTH/SPRITE_WIDTH&&tabP[iChute].forme==7&&tabP[iChute].sens==0)||
							(tabP[iChute].x+1!=GAME_SCREEN_WIDTH/SPRITE_WIDTH&&tabP[iChute].forme==7&&tabP[iChute].sens==1))
						{
							delpintab();
							iPrevPos=tabP[iChute].x;
							tabP[iChute].x++;
							//rafraichie la pièce sauf si elle cogne contre d'autre pièces
							if(addpintab()==false)
							{
								tabP[iChute].x=iPrevPos;
								if(addpintab()==false)
								{
									printf("plantage décalage latéral droit");
									quit=true;
								}
							}
						}
						break;

					case SDLK_DOWN:
						iForward=5;
						break;

					case SDLK_LEFT:
						if(
							//pièce 1
							(tabP[iChute].x>0&&tabP[iChute].forme==1&&tabP[iChute].sens==0)||
							(tabP[iChute].x-1>0&&tabP[iChute].forme==1&&tabP[iChute].sens==1)||
							//pièce 2
							(tabP[iChute].x>0&&tabP[iChute].forme==2&&tabP[iChute].sens==0)||
							(tabP[iChute].x-1>0&&tabP[iChute].forme==2&&tabP[iChute].sens==1)||
							(tabP[iChute].x-1>0&&tabP[iChute].forme==2&&tabP[iChute].sens==2)||
							(tabP[iChute].x-1>0&&tabP[iChute].forme==2&&tabP[iChute].sens==3)||
							// pièce 3
							(tabP[iChute].x-1>0&&tabP[iChute].forme==3&&tabP[iChute].sens==0)||
							(tabP[iChute].x-1>0&&tabP[iChute].forme==3&&tabP[iChute].sens==1)||
							(tabP[iChute].x>0&&tabP[iChute].forme==3&&tabP[iChute].sens==2)||
							(tabP[iChute].x-1>0&&tabP[iChute].forme==3&&tabP[iChute].sens==3)||
							//pièce 4
							(tabP[iChute].x-1>0&&tabP[iChute].forme==4&&(tabP[iChute].sens==0||tabP[iChute].sens==1||tabP[iChute].sens==2))||
							(tabP[iChute].x>0&&tabP[iChute].forme==4&&tabP[iChute].sens==3)||
							//pièce 5
							(tabP[iChute].x!=0&&tabP[iChute].forme==5)||
							//pièce 6
							(tabP[iChute].x-1>0&&tabP[iChute].forme==6&&(tabP[iChute].sens==0||tabP[iChute].sens==1))||
							//pièce 7
							(tabP[iChute].x-1>0&&tabP[iChute].forme==7&&(tabP[iChute].sens==0||tabP[iChute].sens==1)))
						{
							delpintab();
							iPrevPos=tabP[iChute].x;
							tabP[iChute].x--;
							//rafraichie la pièce sauf si elle cogne contre d'autre pièces
							if(addpintab()==false)
							{
								tabP[iChute].x=iPrevPos;
								if(addpintab()==false)
								{
									printf("plantage décalage latéral gauche");
									quit=true;
								}
							}
						}
						break;

                    //Sinon warning
                    default :
                        break;
				}
			}

			if( event.type == SDL_KEYUP && iChute!=-1)
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_DOWN:
						iForward=60;
						break;


                    //Sinon warning
                    default :
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

        //On remplit l'ecran de noir
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );

		//JEU
		if(fps.get_ticks() >  iGameSpeed*iForward)
		{
			//S'il n'y pas de pièce qui tombe
			if(iChute == -1)
			{
			    	        //Si tetris(ligne(s) pleine(s))
                if(checkTetris(HUD.iScore)==true)
                {
                    if(HUD.iScore>=4500)
                        iGameSpeed--;
                    else if(HUD.iScore>=4000)
                        iGameSpeed--;
                    else if(HUD.iScore>=3500)
                        iGameSpeed--;
                    else if(HUD.iScore>=3000)
                        iGameSpeed--;
                    else if(HUD.iScore>=2500)
                        iGameSpeed--;
                    else if(HUD.iScore>=2000)
                        iGameSpeed--;
                    else if(HUD.iScore>=1500)
                        iGameSpeed--;
                    else if(HUD.iScore>=500)
                        iGameSpeed--;
                }

				//crée une nouvelle pièce en cherchant dans le tableau la première place disponible
				for(int i =0;i<TABP_MAX;i++)
				{
					if(tabP[i].active==0)
					{
						tabP[i].initialize();
						iChute = i;
						iForward = 60;
						break;
					}
				}
				if(addpintab()==false)
				{
					dieDisplay();
					printf("gameover");
					gameOver(HUD);
					break;
				}
			}
			//si touche le fond nouvelle pièce
			else if(
				//pièce 1
				(tabP[iChute].sens==0&&tabP[iChute].y+4==GAME_SCREEN_HEIGHT/SPRITE_WIDTH&&tabP[iChute].forme==1)||
				(tabP[iChute].sens==1&&tabP[iChute].y+1==GAME_SCREEN_HEIGHT/SPRITE_WIDTH&&tabP[iChute].forme==1)||
				//pièce 2
				(tabP[iChute].y+2==GAME_SCREEN_HEIGHT/SPRITE_WIDTH&&tabP[iChute].forme==2&&(tabP[iChute].sens==0||tabP[iChute].sens==1||tabP[iChute].sens==2))||
				(tabP[iChute].y+1==GAME_SCREEN_HEIGHT/SPRITE_WIDTH&&tabP[iChute].forme==2&&tabP[iChute].sens==3)||
				//pièce 3
				(tabP[iChute].y+2==GAME_SCREEN_HEIGHT/SPRITE_WIDTH&&tabP[iChute].forme==3&&tabP[iChute].sens==0)||
				(tabP[iChute].y+1==GAME_SCREEN_HEIGHT/SPRITE_WIDTH&&tabP[iChute].forme==3&&tabP[iChute].sens==1)||
				(tabP[iChute].y+2==GAME_SCREEN_HEIGHT/SPRITE_WIDTH&&tabP[iChute].forme==3&&tabP[iChute].sens==2)||
				(tabP[iChute].y+2==GAME_SCREEN_HEIGHT/SPRITE_WIDTH&&tabP[iChute].forme==3&&tabP[iChute].sens==3)||
				//pièce 4
				(tabP[iChute].y+1==GAME_SCREEN_HEIGHT/SPRITE_WIDTH&&tabP[iChute].forme==4&&tabP[iChute].sens==2)||
				(tabP[iChute].y+2==GAME_SCREEN_HEIGHT/SPRITE_WIDTH&&tabP[iChute].forme==4&&(tabP[iChute].sens==0||tabP[iChute].sens==1||tabP[iChute].sens==3))||
				//pièce 5
				(tabP[iChute].y+2==GAME_SCREEN_HEIGHT/SPRITE_WIDTH&&tabP[iChute].forme==5)||
				//pièce 6
				(tabP[iChute].y+2==GAME_SCREEN_HEIGHT/SPRITE_WIDTH&&tabP[iChute].forme==6)||
				//pièce 7
				(tabP[iChute].y+2==GAME_SCREEN_HEIGHT/SPRITE_WIDTH&&tabP[iChute].forme==7))
			{
				tabP[iChute].active=0;
				iChute=-1;
				iForward = 60;
			}
			else if(iChute != -1)
			{
				if(delpintab()==false){return 0;};
				iPrevPos = tabP[iChute].y;
				tabP[iChute].y++;
				if(addpintab()==false)
				{
					//n'a pas pu décendre donc collision, on refait monter la pièce
					tabP[iChute].y=iPrevPos;
					if(addpintab()==false)
					{
						printf("error collision");
						quit=true;
					}
					tabP[iChute].active=0;
					iChute = -1;
				}

			}
			//On demarre le timer fps
			fps.start();
		}

		affichage();
		//interface

		HUD.HUDShowPieces(screen);
		HUD.HUDShowScore(screen);

        //EndGame when you won !!
		if(HUD.iScore>=5000)
        {
            printf("you won a cmd message YEAH !");
            font.showTextFullCenter("you won YEAH !",screen,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
            SDL_Flip(screen);
            waitInput(event);
            quit=true;
        }

        //Mise à jour de l'écran
        if( SDL_Flip( screen ) == -1 )
        {
            return -1;
        }
        //Tant que le timer fps n'est pas assez haut
//        while( fps.get_ticks() < 10000 / 60 )
//		{}
            //On attend...

    }
    //Nettoyage
    clean_up();

    return retVal;
}

bool Game::delpintab()
{
    if (tabP[iChute].forme==1)
	{
		//si debout
		if(tabP[iChute].sens==0)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y+1]=0;
			tab[tabP[iChute].x][tabP[iChute].y+2]=0;
			tab[tabP[iChute].x][tabP[iChute].y+3]=0;
		}
		//sinon couchée
		else
		{
			tab[tabP[iChute].x-1][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x+1][tabP[iChute].y]=0;
			tab[tabP[iChute].x+2][tabP[iChute].y]=0;
		}
	}

	else if (tabP[iChute].forme==2)
	{
		if(tabP[iChute].sens==0)
		{
			tab[tabP[iChute].x][tabP[iChute].y-1]=0;
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y+1]=0;
			tab[tabP[iChute].x+1][tabP[iChute].y+1]=0;
		}
		else if(tabP[iChute].sens==1)
		{
			tab[tabP[iChute].x+1][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y+1]=0;
		}
		else if(tabP[iChute].sens==2)
		{
			tab[tabP[iChute].x-1][tabP[iChute].y-1]=0;
			tab[tabP[iChute].x][tabP[iChute].y-1]=0;
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y+1]=0;
		}
		else if(tabP[iChute].sens==3)
		{
			tab[tabP[iChute].x+1][tabP[iChute].y-1]=0;
			tab[tabP[iChute].x+1][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y]=0;
		}
	}

	else if (tabP[iChute].forme==3)
	{
		if(tabP[iChute].sens==0)
		{
			tab[tabP[iChute].x][tabP[iChute].y-1]=0;
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y+1]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y+1]=0;
		}
		else if(tabP[iChute].sens==1)
		{
			tab[tabP[iChute].x+1][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y-1]=0;
		}
		else if(tabP[iChute].sens==2)
		{
			tab[tabP[iChute].x+1][tabP[iChute].y-1]=0;
			tab[tabP[iChute].x][tabP[iChute].y-1]=0;
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y+1]=0;
		}
		else if(tabP[iChute].sens==3)
		{
			tab[tabP[iChute].x+1][tabP[iChute].y+1]=0;
			tab[tabP[iChute].x+1][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y]=0;
		}
	}
	else if (tabP[iChute].forme==4)
	{
		//T
		if(tabP[iChute].sens==0)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y+1]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y]=0;
			tab[tabP[iChute].x+1][tabP[iChute].y]=0;
		}
		//T gauche
		else if(tabP[iChute].sens==1)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y-1]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y+1]=0;
		}
		//T haut
		else if(tabP[iChute].sens==2)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y-1]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y]=0;
			tab[tabP[iChute].x+1][tabP[iChute].y]=0;
		}
		//T droit
		else if(tabP[iChute].sens==3)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y-1]=0;
			tab[tabP[iChute].x+1][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y+1]=0;
		}
	}
	//carré
	else if (tabP[iChute].forme==5)
	{
		tab[tabP[iChute].x][tabP[iChute].y]=0;
		tab[tabP[iChute].x][tabP[iChute].y+1]=0;
		tab[tabP[iChute].x+1][tabP[iChute].y]=0;
		tab[tabP[iChute].x+1][tabP[iChute].y+1]=0;
	}
	//Z l'envers
	else if (tabP[iChute].forme==6)
	{
		if(tabP[iChute].sens==0)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y+1]=0;
			tab[tabP[iChute].x+1][tabP[iChute].y+1]=0;
		}
		//deuxième sens
		else if(tabP[iChute].sens==1)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y-1]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y+1]=0;
		}
	}
	//Z
	else if (tabP[iChute].forme==7)
	{
		//Z
		if(tabP[iChute].sens==0)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y+1]=0;
			tab[tabP[iChute].x+1][tabP[iChute].y]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y+1]=0;
		}
		//debout
		else if(tabP[iChute].sens==1)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=0;
			tab[tabP[iChute].x][tabP[iChute].y+1]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y]=0;
			tab[tabP[iChute].x-1][tabP[iChute].y-1]=0;
		}
	}
	else
	{
	    printf("appel de la fonct delpintab au mauvais moment");
		return false;
	}
	return true;
}
bool Game::addpintab()
{
	if(iChute == -1)
	{
		printf("appel de la fonct addpintab au mauvais moment");
		return false;
	}

	else if (tabP[iChute].forme==1)
	{
		//si debout
		if(tabP[iChute].sens==0)
		{
			//ne peux pas afficher la pièce s'il y en a une en dessous
			if((tab[tabP[iChute].x][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y+1]||
			tab[tabP[iChute].x][tabP[iChute].y+2]||tab[tabP[iChute].x][tabP[iChute].y+3])!=0)
				return false;
			tab[tabP[iChute].x][tabP[iChute].y]=1;
			tab[tabP[iChute].x][tabP[iChute].y+1]=1;
			tab[tabP[iChute].x][tabP[iChute].y+2]=1;
			tab[tabP[iChute].x][tabP[iChute].y+3]=1;
		}
		//sinon couchée
		else
		{
			if((tab[tabP[iChute].x-1][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y]||
			tab[tabP[iChute].x+1][tabP[iChute].y]||tab[tabP[iChute].x+2][tabP[iChute].y])!=0)
				return false;
			tab[tabP[iChute].x-1][tabP[iChute].y]=1;
			tab[tabP[iChute].x][tabP[iChute].y]=1;
			tab[tabP[iChute].x+1][tabP[iChute].y]=1;
			tab[tabP[iChute].x+2][tabP[iChute].y]=1;

		}
	}

	//L droit
	else if (tabP[iChute].forme==2)
	{
		//si debout
		if(tabP[iChute].sens==0)
		{
			if((tab[tabP[iChute].x][tabP[iChute].y-1]||tab[tabP[iChute].x][tabP[iChute].y]||
			tab[tabP[iChute].x][tabP[iChute].y+1]||tab[tabP[iChute].x+1][tabP[iChute].y+1])!=0)
				return false;
			tab[tabP[iChute].x][tabP[iChute].y-1]=2;
			tab[tabP[iChute].x][tabP[iChute].y]=2;
			tab[tabP[iChute].x][tabP[iChute].y+1]=2;
			tab[tabP[iChute].x+1][tabP[iChute].y+1]=2;
		}
		//si bas
		else if(tabP[iChute].sens==1)
		{
			if((tab[tabP[iChute].x+1][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y]||
			tab[tabP[iChute].x-1][tabP[iChute].y]||tab[tabP[iChute].x-1][tabP[iChute].y+1])!=0)
				return false;
			tab[tabP[iChute].x+1][tabP[iChute].y]=2;
			tab[tabP[iChute].x][tabP[iChute].y]=2;
			tab[tabP[iChute].x-1][tabP[iChute].y]=2;
			tab[tabP[iChute].x-1][tabP[iChute].y+1]=2;
		}
		//si L l'enver droit
		else if(tabP[iChute].sens==2)
		{
			if((tab[tabP[iChute].x-1][tabP[iChute].y-1]||tab[tabP[iChute].x][tabP[iChute].y-1]||
			tab[tabP[iChute].x][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y+1])!=0)
				return false;
			tab[tabP[iChute].x-1][tabP[iChute].y-1]=2;
			tab[tabP[iChute].x][tabP[iChute].y-1]=2;
			tab[tabP[iChute].x][tabP[iChute].y]=2;
			tab[tabP[iChute].x][tabP[iChute].y+1]=2;
		}
		//si L haut
		else if(tabP[iChute].sens==3)
		{
			if((tab[tabP[iChute].x+1][tabP[iChute].y-1]||tab[tabP[iChute].x+1][tabP[iChute].y]||
			tab[tabP[iChute].x][tabP[iChute].y]||tab[tabP[iChute].x-1][tabP[iChute].y])!=0)
				return false;
			tab[tabP[iChute].x+1][tabP[iChute].y-1]=2;
			tab[tabP[iChute].x+1][tabP[iChute].y]=2;
			tab[tabP[iChute].x][tabP[iChute].y]=2;
			tab[tabP[iChute].x-1][tabP[iChute].y]=2;
		}
	}

	//L gauche
	else if (tabP[iChute].forme==3)
	{
		//si debout
		if(tabP[iChute].sens==0)
		{
			if((tab[tabP[iChute].x][tabP[iChute].y-1]||tab[tabP[iChute].x][tabP[iChute].y]||
			tab[tabP[iChute].x][tabP[iChute].y+1]||tab[tabP[iChute].x-1][tabP[iChute].y+1])!=0)
					return false;
			tab[tabP[iChute].x][tabP[iChute].y-1]=3;
			tab[tabP[iChute].x][tabP[iChute].y]=3;
			tab[tabP[iChute].x][tabP[iChute].y+1]=3;
			tab[tabP[iChute].x-1][tabP[iChute].y+1]=3;
		}
		//si couché haut
		else if(tabP[iChute].sens==1)
		{
			if((tab[tabP[iChute].x+1][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y]||
			tab[tabP[iChute].x-1][tabP[iChute].y]||tab[tabP[iChute].x-1][tabP[iChute].y-1])!=0)
					return false;
			tab[tabP[iChute].x+1][tabP[iChute].y]=3;
			tab[tabP[iChute].x][tabP[iChute].y]=3;
			tab[tabP[iChute].x-1][tabP[iChute].y]=3;
			tab[tabP[iChute].x-1][tabP[iChute].y-1]=3;
		}
		//si debout l'envers
		else if(tabP[iChute].sens==2)
		{
			if((tab[tabP[iChute].x+1][tabP[iChute].y-1]||tab[tabP[iChute].x][tabP[iChute].y-1]||
			tab[tabP[iChute].x][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y+1])!=0)
					return false;
			tab[tabP[iChute].x+1][tabP[iChute].y-1]=3;
			tab[tabP[iChute].x][tabP[iChute].y-1]=3;
			tab[tabP[iChute].x][tabP[iChute].y]=3;
			tab[tabP[iChute].x][tabP[iChute].y+1]=3;
		}
		//si couché bas
		else if(tabP[iChute].sens==3)
		{
			if((tab[tabP[iChute].x+1][tabP[iChute].y+1]||tab[tabP[iChute].x+1][tabP[iChute].y]||
			tab[tabP[iChute].x][tabP[iChute].y]||tab[tabP[iChute].x-1][tabP[iChute].y])!=0)
					return false;
			tab[tabP[iChute].x+1][tabP[iChute].y+1]=3;
			tab[tabP[iChute].x+1][tabP[iChute].y]=3;
			tab[tabP[iChute].x][tabP[iChute].y]=3;
			tab[tabP[iChute].x-1][tabP[iChute].y]=3;
		}
	}
	//T
	else if (tabP[iChute].forme==4)
	{
		//T
		if(tabP[iChute].sens==0)
		{
			if((tab[tabP[iChute].x][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y+1]||
			tab[tabP[iChute].x-1][tabP[iChute].y]||tab[tabP[iChute].x+1][tabP[iChute].y])!=0)
				return false;
			tab[tabP[iChute].x][tabP[iChute].y]=4;
			tab[tabP[iChute].x][tabP[iChute].y+1]=4;
			tab[tabP[iChute].x-1][tabP[iChute].y]=4;
			tab[tabP[iChute].x+1][tabP[iChute].y]=4;
		}
		//T gauche
		else if(tabP[iChute].sens==1)
		{
			if((tab[tabP[iChute].x][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y-1]||
			tab[tabP[iChute].x-1][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y+1])!=0)
				return false;
			tab[tabP[iChute].x][tabP[iChute].y]=4;
			tab[tabP[iChute].x][tabP[iChute].y-1]=4;
			tab[tabP[iChute].x-1][tabP[iChute].y]=4;
			tab[tabP[iChute].x][tabP[iChute].y+1]=4;
		}
		//T haut
		else if(tabP[iChute].sens==2)
		{
			if((tab[tabP[iChute].x][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y-1]||
			tab[tabP[iChute].x-1][tabP[iChute].y]||tab[tabP[iChute].x+1][tabP[iChute].y])!=0)
				return false;
			tab[tabP[iChute].x][tabP[iChute].y]=4;
			tab[tabP[iChute].x][tabP[iChute].y-1]=4;
			tab[tabP[iChute].x-1][tabP[iChute].y]=4;
			tab[tabP[iChute].x+1][tabP[iChute].y]=4;
		}
		//T droit
		else if(tabP[iChute].sens==3)
		{
			if((tab[tabP[iChute].x][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y-1]||
			tab[tabP[iChute].x+1][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y+1])!=0)
				return false;
			tab[tabP[iChute].x][tabP[iChute].y]=4;
			tab[tabP[iChute].x][tabP[iChute].y-1]=4;
			tab[tabP[iChute].x+1][tabP[iChute].y]=4;
			tab[tabP[iChute].x][tabP[iChute].y+1]=4;
		}
	}
	//carré
	else if(tabP[iChute].forme==5)
	{
		if((tab[tabP[iChute].x][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y+1]||
		tab[tabP[iChute].x+1][tabP[iChute].y]||tab[tabP[iChute].x+1][tabP[iChute].y+1])!=0)
					return false;
		tab[tabP[iChute].x][tabP[iChute].y]=5;
		tab[tabP[iChute].x][tabP[iChute].y+1]=5;
		tab[tabP[iChute].x+1][tabP[iChute].y]=5;
		tab[tabP[iChute].x+1][tabP[iChute].y+1]=5;
	}
	//Z l'envers
	else if (tabP[iChute].forme==6)
	{
		//Z
		if(tabP[iChute].sens==0)
		{
			if((tab[tabP[iChute].x][tabP[iChute].y]||
			tab[tabP[iChute].x-1][tabP[iChute].y]||
			tab[tabP[iChute].x][tabP[iChute].y+1]||
			tab[tabP[iChute].x+1][tabP[iChute].y+1])!=0)
					return false;
			tab[tabP[iChute].x][tabP[iChute].y]=6;
			tab[tabP[iChute].x-1][tabP[iChute].y]=6;
			tab[tabP[iChute].x][tabP[iChute].y+1]=6;
			tab[tabP[iChute].x+1][tabP[iChute].y+1]=6;
		}
		//debout
		else if(tabP[iChute].sens==1)
		{
			if((tab[tabP[iChute].x][tabP[iChute].y]||
			tab[tabP[iChute].x][tabP[iChute].y-1]||
			tab[tabP[iChute].x-1][tabP[iChute].y]||
			tab[tabP[iChute].x-1][tabP[iChute].y+1])!=0)
					return false;
			tab[tabP[iChute].x][tabP[iChute].y]=6;
			tab[tabP[iChute].x][tabP[iChute].y-1]=6;
			tab[tabP[iChute].x-1][tabP[iChute].y]=6;
			tab[tabP[iChute].x-1][tabP[iChute].y+1]=6;
		}
	}
	//Z
	else if (tabP[iChute].forme==7)
	{
		//Z
		if(tabP[iChute].sens==0)
		{
			if((tab[tabP[iChute].x][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y+1]||
			tab[tabP[iChute].x+1][tabP[iChute].y]||tab[tabP[iChute].x-1][tabP[iChute].y+1])!=0)
					return false;
			tab[tabP[iChute].x][tabP[iChute].y]=7;
			tab[tabP[iChute].x][tabP[iChute].y+1]=7;
			tab[tabP[iChute].x+1][tabP[iChute].y]=7;
			tab[tabP[iChute].x-1][tabP[iChute].y+1]=7;
		}
		//debout
		else if(tabP[iChute].sens==1)
		{
			if((tab[tabP[iChute].x][tabP[iChute].y]||tab[tabP[iChute].x][tabP[iChute].y+1]||
			tab[tabP[iChute].x-1][tabP[iChute].y]||tab[tabP[iChute].x-1][tabP[iChute].y-1])!=0)
					return false;
			tab[tabP[iChute].x][tabP[iChute].y]=7;
			tab[tabP[iChute].x][tabP[iChute].y+1]=7;
			tab[tabP[iChute].x-1][tabP[iChute].y]=7;
			tab[tabP[iChute].x-1][tabP[iChute].y-1]=7;
		}
	}
	return true;
}

//pas très clean, on le fait déjà avec pintab() sauf que j'ai enlevé les condition if
void Game::dieDisplay()
{
	if (tabP[iChute].forme==1)
	{
		//si debout
		if(tabP[iChute].sens==0)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=1;
			tab[tabP[iChute].x][tabP[iChute].y+1]=1;
			tab[tabP[iChute].x][tabP[iChute].y+2]=1;
			tab[tabP[iChute].x][tabP[iChute].y+3]=1;
		}
		//sinon couchée
		else
		{
			tab[tabP[iChute].x-1][tabP[iChute].y]=1;
			tab[tabP[iChute].x][tabP[iChute].y]=1;
			tab[tabP[iChute].x+1][tabP[iChute].y]=1;
			tab[tabP[iChute].x+2][tabP[iChute].y]=1;

		}
	}

	//L droit
	else if (tabP[iChute].forme==2)
	{
		//si debout
		if(tabP[iChute].sens==0)
		{
			tab[tabP[iChute].x][tabP[iChute].y-1]=2;
			tab[tabP[iChute].x][tabP[iChute].y]=2;
			tab[tabP[iChute].x][tabP[iChute].y+1]=2;
			tab[tabP[iChute].x+1][tabP[iChute].y+1]=2;
		}
		//si bas
		else if(tabP[iChute].sens==1)
		{
			tab[tabP[iChute].x+1][tabP[iChute].y]=2;
			tab[tabP[iChute].x][tabP[iChute].y]=2;
			tab[tabP[iChute].x-1][tabP[iChute].y]=2;
			tab[tabP[iChute].x-1][tabP[iChute].y+1]=2;
		}
		//si L l'enver droit
		else if(tabP[iChute].sens==2)
		{
			tab[tabP[iChute].x-1][tabP[iChute].y-1]=2;
			tab[tabP[iChute].x][tabP[iChute].y-1]=2;
			tab[tabP[iChute].x][tabP[iChute].y]=2;
			tab[tabP[iChute].x][tabP[iChute].y+1]=2;
		}
		//si L haut
		else if(tabP[iChute].sens==3)
		{
			tab[tabP[iChute].x+1][tabP[iChute].y-1]=2;
			tab[tabP[iChute].x+1][tabP[iChute].y]=2;
			tab[tabP[iChute].x][tabP[iChute].y]=2;
			tab[tabP[iChute].x-1][tabP[iChute].y]=2;
		}
	}

	//L gauche
	else if (tabP[iChute].forme==3)
	{
		//si debout
		if(tabP[iChute].sens==0)
		{
			tab[tabP[iChute].x][tabP[iChute].y-1]=3;
			tab[tabP[iChute].x][tabP[iChute].y]=3;
			tab[tabP[iChute].x][tabP[iChute].y+1]=3;
			tab[tabP[iChute].x-1][tabP[iChute].y+1]=3;
		}
		//si couché haut
		else if(tabP[iChute].sens==1)
		{
			tab[tabP[iChute].x+1][tabP[iChute].y]=3;
			tab[tabP[iChute].x][tabP[iChute].y]=3;
			tab[tabP[iChute].x-1][tabP[iChute].y]=3;
			tab[tabP[iChute].x-1][tabP[iChute].y-1]=3;
		}
		//si debout l'envers
		else if(tabP[iChute].sens==2)
		{
			tab[tabP[iChute].x+1][tabP[iChute].y-1]=3;
			tab[tabP[iChute].x][tabP[iChute].y-1]=3;
			tab[tabP[iChute].x][tabP[iChute].y]=3;
			tab[tabP[iChute].x][tabP[iChute].y+1]=3;
		}
		//si couché bas
		else if(tabP[iChute].sens==3)
		{
			tab[tabP[iChute].x+1][tabP[iChute].y+1]=3;
			tab[tabP[iChute].x+1][tabP[iChute].y]=3;
			tab[tabP[iChute].x][tabP[iChute].y]=3;
			tab[tabP[iChute].x-1][tabP[iChute].y]=3;
		}
	}
	//T
	else if (tabP[iChute].forme==4)
	{
		//T
		if(tabP[iChute].sens==0)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=4;
			tab[tabP[iChute].x][tabP[iChute].y+1]=4;
			tab[tabP[iChute].x-1][tabP[iChute].y]=4;
			tab[tabP[iChute].x+1][tabP[iChute].y]=4;
		}
		//T gauche
		else if(tabP[iChute].sens==1)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=4;
			tab[tabP[iChute].x][tabP[iChute].y-1]=4;
			tab[tabP[iChute].x-1][tabP[iChute].y]=4;
			tab[tabP[iChute].x][tabP[iChute].y+1]=4;
		}
		//T haut
		else if(tabP[iChute].sens==2)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=4;
			tab[tabP[iChute].x][tabP[iChute].y-1]=4;
			tab[tabP[iChute].x-1][tabP[iChute].y]=4;
			tab[tabP[iChute].x+1][tabP[iChute].y]=4;
		}
		//T droit
		else if(tabP[iChute].sens==3)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=4;
			tab[tabP[iChute].x][tabP[iChute].y-1]=4;
			tab[tabP[iChute].x+1][tabP[iChute].y]=4;
			tab[tabP[iChute].x][tabP[iChute].y+1]=4;
		}
	}
	//carré
	else if(tabP[iChute].forme==5)
	{
		tab[tabP[iChute].x][tabP[iChute].y]=5;
		tab[tabP[iChute].x][tabP[iChute].y+1]=5;
		tab[tabP[iChute].x+1][tabP[iChute].y]=5;
		tab[tabP[iChute].x+1][tabP[iChute].y+1]=5;
	}
	//Z l'envers
	else if (tabP[iChute].forme==6)
	{
		//Z
		if(tabP[iChute].sens==0)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=6;
			tab[tabP[iChute].x-1][tabP[iChute].y]=6;
			tab[tabP[iChute].x][tabP[iChute].y+1]=6;
			tab[tabP[iChute].x+1][tabP[iChute].y+1]=6;
		}
		//debout
		else if(tabP[iChute].sens==1)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=6;
			tab[tabP[iChute].x][tabP[iChute].y-1]=6;
			tab[tabP[iChute].x-1][tabP[iChute].y]=6;
			tab[tabP[iChute].x-1][tabP[iChute].y+1]=6;
		}
	}
	//Z
	else if (tabP[iChute].forme==7)
	{
		//Z
		if(tabP[iChute].sens==0)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=7;
			tab[tabP[iChute].x][tabP[iChute].y+1]=7;
			tab[tabP[iChute].x+1][tabP[iChute].y]=7;
			tab[tabP[iChute].x-1][tabP[iChute].y+1]=7;
		}
		//debout
		else if(tabP[iChute].sens==1)
		{
			tab[tabP[iChute].x][tabP[iChute].y]=7;
			tab[tabP[iChute].x][tabP[iChute].y+1]=7;
			tab[tabP[iChute].x-1][tabP[iChute].y]=7;
			tab[tabP[iChute].x-1][tabP[iChute].y-1]=7;
		}
	}
}

void Game::affichage()
{
	for (int i = 0; i < GAME_SCREEN_HEIGHT/SPRITE_WIDTH;i++)
	{
		for (int u = 0; u < GAME_SCREEN_WIDTH/SPRITE_WIDTH;u++)
		{
			switch(tab[u][i])
			{
				case 0:
					apply_surface(u*SPRITE_WIDTH,i*SPRITE_WIDTH,surf_form0,screen,NULL);
					break;

				case 1:
					apply_surface(u*SPRITE_WIDTH,i*SPRITE_WIDTH,surf_form1,screen,NULL);
					break;

				case 2:
					apply_surface(u*SPRITE_WIDTH,i*SPRITE_WIDTH,surf_form2,screen,NULL);
					break;

				case 3:
					apply_surface(u*SPRITE_WIDTH,i*SPRITE_WIDTH,surf_form3,screen,NULL);
					break;

				case 4:
					apply_surface(u*SPRITE_WIDTH,i*SPRITE_WIDTH,surf_form4,screen,NULL);
					break;

				case 5:
					apply_surface(u*SPRITE_WIDTH,i*SPRITE_WIDTH,surf_form5,screen,NULL);
					break;

				case 6:
					apply_surface(u*SPRITE_WIDTH,i*SPRITE_WIDTH,surf_form6,screen,NULL);
					break;

				case 7:
					apply_surface(u*SPRITE_WIDTH,i*SPRITE_WIDTH,surf_form7,screen,NULL);
					break;

                case 8:
                    apply_surface(u*SPRITE_WIDTH,i*SPRITE_WIDTH,surf_gameOver,screen,NULL);
					break;
			}
		}
	}
}

void Game::gameOver(HUD_Game HUD)
{
    Timer timerGameOver;
    timerGameOver.start();

    int i=0;
    bool gameOverContinue=true;
    while(gameOverContinue)
    {
        if(timerGameOver.get_ticks()>iGOSpeed)
        {
            for (int j=0;j<GRID_WIDTH;j++)
            {
                if(j%2)
                {
                    tab[j][i]=8;
                    tab[j-1][GRID_HEIGHT-1-i]=8;
                }
            }
            affichage();
            HUD.HUDShowPieces(screen);
            HUD.HUDShowScore(screen);
            i++;
            if(i>=GRID_HEIGHT)
                gameOverContinue=false;
            SDL_Flip(screen);
            timerGameOver.start();
        }
    }
    BitmapFont font = surfFont;

    font.showTextFullCenterAndBox("GAME OVER",screen,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,16,19,58);
    SDL_Flip(screen);
    waitInput(event);
    font.fontDelete();
}

bool Game::checkTetris(int &iScore)
{
	int nbFullLine=0;
	int lastFullLine=GAME_SCREEN_HEIGHT/SPRITE_WIDTH;
	for (int i = 0; i < GAME_SCREEN_HEIGHT/SPRITE_WIDTH;i++)
	{
		int stock = 1;
		//parcours toute la ligne pour voir si elle est pleine
		for (int u = 0; u < GAME_SCREEN_WIDTH/SPRITE_WIDTH;u++)
		{
			if(tab[u][i]<stock)
				stock=0;
		}
		//la ligne est pleine
		if (stock==1)
		{
			nbFullLine++;
			if(lastFullLine>i)
				lastFullLine = i;
		}
	}
	//Descend les lignes
	//part depuis la première ligne à déscendre
	if(nbFullLine>0)
	{
		for (int i = lastFullLine-1; i > 0;i--)
		{
			//parcours toute la ligne pour descendre chaque carrée de nbFullLine
			for (int u = 0; u < GAME_SCREEN_WIDTH/SPRITE_WIDTH;u++)
			{
				tab[u][i+nbFullLine]=tab[u][i];
				tab[u][i]=0;
			}
		}
		iScore+= nbFullLine*nbFullLine*100;
		printf("\nScore : %d\n",iScore);
		return true;
	}
	return false;
}

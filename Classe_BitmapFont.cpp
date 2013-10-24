#include "Classe_BitmapFont.h"
#include <sstream>
#include <string>
#include "SDL.h"

using namespace std;

BitmapFont::BitmapFont()
{
    //Met bitmap a NULL
    bitmap = NULL;
}

BitmapFont::BitmapFont( SDL_Surface *surface )
{
    //Construit le font
    build_font( surface );
}

void BitmapFont::build_font( SDL_Surface *surface )
{
    //Si la surface est NULL
    if( surface == NULL )
    {
		//SDL_Exit("chargement de la font echouer");
        return;
    }

    //Recupere le bitmap
    bitmap = surface;

    //Mise en place de la couleur du background (fond)
    //Uint32 bgColor = SDL_MapRGB( bitmap->format, 0, 0xFF, 0xFF );

    //Dimension des cellules
    int cellW = bitmap->w / 16;
    int cellH = bitmap->h / 16;

    //Le caractere courant
    int currentChar = 0;

    //On parcours les lignes des cellules
    for( int rows = 0; rows < 16; rows++ )
    {
        //On parcours les colonnes des cellules
        for( int cols = 0; cols < 16; cols++ )
        {
            //caractere courant
            chars[ currentChar ].x = cellW * cols;
            chars[ currentChar ].y = cellH * rows;

            //Mise en place des dimensions du caractere
            chars[ currentChar ].w = cellW;
            chars[ currentChar ].h = cellH;

//            //On parcours les colonnes des pixels
//            for( int pCol = 0; pCol < cellW; pCol++ )
//            {
//                //On parcours les lignes des pixels
//                for( int pRow = 0; pRow < cellH; pRow++ )
//                {
//                    //Recupere les coordonnees du pixel
//                    int pX = ( cellW * cols ) + pCol;
//                    int pY = ( cellH * rows ) + pRow;
//
//                    //Si un pixel non "colorkey" est trouve
//                    if( get_pixel32( pX, pY, bitmap ) != bgColor )
//                    {
//                        //Coordonnee x
//                        chars[ currentChar ].x = pX;
//
//                        //On arrete la boucle
//                        pCol = cellW;
//                        pRow = cellH;
//                    }
//                }
//            }
//
//            //On parcours les colonnes des pixels
//            for( int pCol_w = cellW - 1; pCol_w >= 0; pCol_w-- )
//            {
//                //On parcours les lignes des pixels
//                for( int pRow_w = 0; pRow_w < cellH; pRow_w++ )
//                {
//                    //Recupere les coordonnees du pixel
//                    int pX = ( cellW * cols ) + pCol_w;
//                    int pY = ( cellH * rows ) + pRow_w;
//
//                    //Si un pixel non "colorkey" est trouve
//                    if( get_pixel32( pX, pY, bitmap ) != bgColor )
//                    {
//                        //longueur du caractere courant
//                        chars[ currentChar ].w = ( pX - chars[ currentChar ].x ) + 5;
//
//                        //On arrete la boucle
//                        pCol_w = -1;
//                        pRow_w = cellH;
//                    }
//                }
//            }

            //caractere suivant
            currentChar++;
        }
    }
}

void BitmapFont::show_text( int x, int y, std::string text, SDL_Surface *surface )
{
    //coordonnees temporaires

    int X = x, Y = y;

    //verification que le font a bien ete construit
    if( bitmap != NULL )
    {
        //On parcours le texte
        for( int show = 0; text[ show ] != '\0'; show++ )
        {
            //Si le caractere courant est un espace
            if( text[ show ] == ' ' )
            {
                //On bouge de la taille d'un caractere
                X += bitmap->w / 16;
            }
            //Si le caractere courant est un "newline"
            else if( text[ show ] == '\n' )
            {
                //On descent
                Y += bitmap->h / 16;

                //On revient en arriere
                X = x;
            }
            else
            {
                //recupere la valeur ASCII du caractere
                int ascii = (int)text[ show ];

                //Affiche le caractere
                //Pour l'apply j'ai changer le x pour qu'ils soit toujours centré
                //apply_surface( X, Y, bitmap, surface, &chars[ ascii ] );
                apply_surface( X, Y, bitmap, surface, &chars[ ascii ] );

                //On bouge de la longueur du caractere + un pas de un pixel
                X += chars[ ascii ].w;
            }
        }
    }
    else
        printf("error bitmapfont");

}

void BitmapFont::show_text_center(int y, int w, std::string text, SDL_Surface *surface)
{

    int XMax=0;//nombre max de cara
    for( int show = 0; text[ show ] != '\0'; show++ )
    {
        XMax+=bitmap->w / 16;
    }
    int x=w/2-XMax/2;
    show_text(x,y,text,surface);
}

void BitmapFont::showTextFullCenter(std::string text, SDL_Surface *surface,int xZone, int yZone, int wZone, int hZone)
{
    int nbMaxLine=1;
    int actuLine=0;//0 car indice de tableau
    //compte le nombre de lignes
    for (int i=0;text[i]!='\0';i++)
    {
    	if(text[i]=='\n')
            nbMaxLine++;
    }

    string strLine;
    stringstream stream(text);
    while( getline(stream, strLine, '\n') )
    {
        int wText = strLine.size()*bitmap->w / 16;
        int hText = actuLine*bitmap->h/16;
        int x=xZone+wZone/2-wText/2;
        int y=yZone+hZone/2+hText-(nbMaxLine*bitmap->h/16)/2;//-bitmap->h/32;
        show_text(x,y,strLine,surface);
        actuLine++;
    }
}

void BitmapFont::showTextFullCenterAndBox(std::string text, SDL_Surface *surface,int xZone, int yZone, int wZone, int hZone,int R, int G, int B)
{
    int nbMaxLine=1;
    int maxCara = 0;
    int actuLine=0;//0 car indice de tableau
    //compte le nombre de lignes
    for (int i=0, tempMaxCara=1;text[i]!='\0';i++,tempMaxCara++)
    {
    	if(text[i]=='\n')
    	{
    	    nbMaxLine++;
    	    if(tempMaxCara>maxCara)
    	    {
                maxCara=tempMaxCara;
    	    }
            tempMaxCara=0;
    	}
    	else if(text[i+1]=='\0'&&nbMaxLine==1)
    	{
    	    maxCara=tempMaxCara;
    	}
    }
    int wBox=maxCara*bitmap->w / 16,hBox=nbMaxLine*bitmap->h/16;
    int xBox=xZone+wZone/2-wBox/2,yBox=yZone+hZone/2-hBox/2-bitmap->h/32;
    SDL_Surface *surfBox = SDL_CreateRGBSurface(0,wBox,hBox,32,0,0,0,0);
    SDL_FillRect(surfBox, NULL, SDL_MapRGB(surfBox->format, R,G,B));
    apply_surface(xBox,yBox,surfBox,surface,NULL);
    SDL_FreeSurface(surfBox);

    string strLine;
    stringstream stream(text);
    while( getline(stream, strLine, '\n') )
    {
        int wText = strLine.size()*bitmap->w / 16;
        int hText = actuLine*bitmap->h/16;
        int x=xZone-wText/2+wZone/2;
        int y=yZone+hZone/2+hText-(nbMaxLine*bitmap->h/16)/2-bitmap->h/32;
        show_text(x,y,strLine,surface);
        actuLine++;
    }
}
////pas fini voir inutile
//void BitmapFont::show_text_right( int y, std::string text, SDL_Surface *surface)
//{
//    if( bitmap != NULL )
//    {
//
//    }
//}
//
//void BitmapFont::show_text_left(int y, std::string text, SDL_Surface *surface)
//{
//    if( bitmap != NULL )
//    {
//
//    }
//}

//void BitmapFont::TextBox(SDL_Surface *screen,SDL_Event event,std::string text)
//{
//    //musique
//    //Music musicDing("Sounds/text_beep.mp3");
//
//    Timer time;
//    time.start();
//
//    SDL_Surface *tbx=NULL;
//    tbx=load_image("sprites/textbox.png");
//
//    //endroit où le text doit commence
//    int beginXText = 30;
//    int beginYText = GAME_SCREEN_HEIGHT-tbx->h;
//
//    apply_surface(10,GAME_SCREEN_HEIGHT-tbx->h-15,tbx,screen,NULL);
//    // insère les caractères petit à petit
//
//    int X = beginXText;
//    int Y = beginYText;
//    int Yini = beginYText;
//    int lastSpace = -1;
//    const int hLigne = bitmap->h / 16;
//    const int caraMaxLigne = 20;
//    int iVitesseTbx = 2500;
//
//    //verification que le font a bien ete construit
//    if( bitmap != NULL )
//    {
//        //joue le son
//        //musicDing.playMusic(true);
//        //On parcours le texte une première fois pour l'addapté à la taille de la textbox
//        int cara=0;
//        for( int show = 0; text[ show ] != '\0'; show++ )
//        {
//            //Si le caractere courant est un espace
//            if( text[ show ] == ' ' )
//            {
//                lastSpace = show;
//            }
//            if(cara>caraMaxLigne)
//            {
//                cara = 0;
//                text[lastSpace]='\n';
//            }
//            cara++;
//        }
//        for( int show = 0; text[ show ] != '\0'; show++ )
//        {
//            //accéléré le text s'il y a une touche préssé
//            SDL_PollEvent(&event);//Récupération de l'évènement dans event
//            switch(event.type)//Test du type d'évènement
//            {
//                case SDL_KEYDOWN:
//                    iVitesseTbx = 500;
//                    break;
//
//                case SDL_KEYUP:
//                    iVitesseTbx = 2500;
//                    break;
//
//                case SDL_MOUSEBUTTONDOWN:
//                    iVitesseTbx = 500;
//                    break;
//
//                case SDL_MOUSEBUTTONUP:
//                    iVitesseTbx = 2500;
//                    break;
//            }
//
//            //Si le caractere courant est un espace
//            if( text[ show ] == ' ' )
//            {
//                //On bouge de la taille d'un caractere
//                X += bitmap->w / 32;
//            }
//            //Si le caractere courant est un "newline"
//            else if( text[ show ] == '\n' )
//            {
//                //Si c'est le dernier retour à la ligne on attend une touche préssé
//                if (Y>=Yini+3*hLigne)
//                {
//                	show_text(X,Y,"...",screen);
//                    SDL_Flip(screen);
//                    Y-=3*hLigne;
//                    X = beginXText;
//                    //stop le son
//                    //musicDing.stopMusic();
//                    waitInput(event);
//                    //rejoue le son
//                    //musicDing.playMusic(true);
//                    //réaffiche la tbx
//                    apply_surface(10,GAME_SCREEN_HEIGHT-tbx->h-15,tbx,screen,NULL);
//                }
//                else
//                {
//                    //On descent
//                    Y += hLigne;
//
//                    //On revient en arriere
//                    X = beginXText;
//                }
//            }
//            else
//            {
//                //recupere la valeur ASCII du caractere
//                int ascii = (int)text[ show ];
//
//                //Affiche le caractere
//                //Pour l'apply j'ai changer le x pour qu'ils soit toujours centré
//                //apply_surface( X, Y, bitmap, surface, &chars[ ascii ] );
//                apply_surface( X, Y, bitmap, screen, &chars[ ascii ] );
//
//                SDL_Flip(screen);
//                time.start();
//                while( time.get_ticks() < iVitesseTbx / FRAMES_PER_SECOND )
//                {
//                    //on attend
//                }
//
//                //On bouge de la longueur du caractere + un pas de un pixel
//                X += chars[ ascii ].w + 1;
//            }
//
//        }
//        musicDing.stopMusic();
//    }
//    else
//        printf("error bitmapfont");
//
//    //fin des caractères petit à petit
//    //show_text(beginXText,beginYText,text,screen);
//
//    SDL_Flip(screen);
//    waitInput(event);
//}

//void BitmapFont::waitInput(SDL_Event event)
//{
//    int waitInp = true;
//    while(waitInp)
//    {
//        SDL_PollEvent(&event);//Récupération de l'évènement dans event
//        switch(event.type)//Test du type d'évènement
//        {
//            case SDL_KEYDOWN:
//                waitInp=false;
//                break;
//
//            case SDL_MOUSEBUTTONDOWN:
//                waitInp=false;
//                break;
//        }
//    }
//}

void BitmapFont::fontDelete()
{
    SDL_FreeSurface(bitmap);
}

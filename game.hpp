//Les fichiers d'entête
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <string>
#include <sstream>

#include <vector>
using namespace std;

//Classe
#include "constantes.h"
#include "classic_function.hpp"
#include "Classe_Timer.h"
#include "Classe_Piece.h"
#include "Classe_BitmapFont.h"
#include "HUD_Game.hpp"

class Game//:HUD_Game
{
    private:
        SDL_Surface *screen , *surf_form0, *surf_form1, *surf_form2, *surf_form3, *surf_form4,
        *surf_form5,*surf_form6,*surf_form7,*surf_gameOver;
        SDL_Surface *surfFont;
        //La musique qui sera joué
        Mix_Music *music;

        //La structure d'événements que nous allons utiliser
        SDL_Event event;

        //grille
        vector<vector<char> > tab;
//        vector<vector<int> > items[5][5];
//        char **tab;

        int FRAMES_PER_SECOND;

        int iChute;
        int iGameSpeed;
        int iForward;
        //int iScore;
        int iPrevSens;
        int iPrevPos;
        //tableau de pièce
        Piece tabP[TABP_MAX];

        void clean_up();
        bool load_files();
        bool init();
        void dieDisplay();
        void affichage();
        void gameOver(HUD_Game HUD);int iGOSpeed;
        bool checkTetris(int &iScore);

        bool addpintab();
        bool delpintab();

        void refreshVariable();


    public:
        Game();
        int Start();
};


#include "SDL/SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#include "classic_function.hpp"
#include "Constantes.h"
#include "global.hpp"
#include "Button.hpp"

class Menu
{
    private:
        //Le nombre de frame par seconde
        //int FRAMES_PER_SECOND = 2;
        SDL_Surface *screen;
        //La musique qui sera joué
        Mix_Music *music;
        //La structure d'événements que nous allons utiliser
        SDL_Event event;

    public:
        Menu();
        int Start();

};

#include "SDL/SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#include "global.hpp"

class Menu_Option
{
    private:
        SDL_Surface *screen;
        //La musique qui sera joué
        Mix_Music *music;
        //La structure d'événements que nous allons utiliser
        SDL_Event event;

    public:
        Menu_Option();
        int Start();
};

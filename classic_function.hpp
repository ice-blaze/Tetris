#include "SDL/SDL.h"
#include <string>

#ifndef CLASSIC_FUNCTION_H
#define CLASSIC_FUNCTION_H

int MyRand(int a, int b);
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
SDL_Surface *load_image( std::string filename );
Uint32 get_pixel32( int x, int y, SDL_Surface *surface );
void waitInput(SDL_Event event);
int collision (int x1,int y1, int w1, int h1,
               int x2, int y2, int w2, int h2);

#endif

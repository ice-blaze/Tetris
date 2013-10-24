
#include "Classe_Piece.h"
#include "classic_function.hpp"

int Piece::nextForme = MyRand(1,7);

void Piece::initialize()
{
	active = 1;
	forme = nextForme;
	nextForme=MyRand(1,7);
	sens=0;
	y = 1;
	//centre de la pièce pas trop bas on la remonte
	if (forme==1||forme==5)
		y = 0;
	x = (GAME_SCREEN_WIDTH/SPRITE_WIDTH)/2;
}

void Piece::del()
{
	active = 0;
}

bool Piece::changePos(int posX, int posY)
{
	if(posX>GAME_SCREEN_WIDTH/SPRITE_WIDTH||posY>GAME_SCREEN_HEIGHT/SPRITE_WIDTH||(posY&&posX)<0)
	{
		printf("changement de position ERROR");
		return false;
	}
	x=posX;
	y=posY;
	printf("changement de position OK");
	return true;
}

bool Piece::changeSens()
{
	//formes qui tournent que deux fois
	if(forme==1||forme==6||forme==7)
	{
		if(sens==1)
			sens=0;
		else
			sens++;
	}
	//formes qui tournent 4 fois
	else if (forme == 2||forme==3||forme==4)
	{
		if(sens==3)
			sens=0;
		else
			sens++;
	}

	printf("changement de sens OK %d\n",sens);
	return true;
}


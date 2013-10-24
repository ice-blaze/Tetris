#include "Constantes.h"
#include "global.hpp"

#ifndef CLASSE_PIECE_H
#define CLASSE_PIECE_H

class Piece
{
	private:

	public:
	int x;
	int y;
	static int nextForme;
	char forme;
	char sens;
	char active;

	void initialize();
	void del();
	bool changePos(int posX, int posY);
	bool changeSens(); // 1 = droite / 2 = gauche
};

#endif

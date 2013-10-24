#include "Constantes.h"
#include "game.hpp"
#include "Menu.hpp"
#include "Menu_Option.hpp"


int main ( int argc, char** argv )
{
    Menu mainMenu;
    Menu_Option menuOpt1;
    Game tetrisGame;
    int gameContinue = mainMenu.Start();

    while (gameContinue>=0)
    {
        if(gameContinue==0)
        {
            gameContinue = mainMenu.Start();
        }
        else if(gameContinue==1)
        {
            gameContinue = tetrisGame.Start();
        }
        else if (gameContinue==2)
        {
            gameContinue = menuOpt1.Start();
        }
    }

    return 0;
}

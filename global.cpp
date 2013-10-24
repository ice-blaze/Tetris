#include "Constantes.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int GRID_WIDTH = 10;
int GRID_HEIGHT = 20;
int GAME_SCREEN_WIDTH = SPRITE_WIDTH*GRID_WIDTH;
int GAME_SCREEN_HEIGHT = SPRITE_WIDTH*GRID_HEIGHT;
int SCREEN_WIDTH = GAME_SCREEN_WIDTH+150;
int SCREEN_HEIGHT = GAME_SCREEN_HEIGHT;

int INTERF_X = SCREEN_WIDTH-INTERF_WIDTH-20;

int INTERF_MAIN_PIECE_X = INTERF_X - INTERF_WIDTH_BORDER + INTERF_WIDTH /2;
int INTERF_MAIN_PIECE_Y = INTERF_Y + INTERF_WIDTH_BORDER + SPRITE_WIDTH + 3;

void loadCFG()
{
    vector<int> vctResult;
    string line;

    ifstream cfgFile("config/map.cfg");
    if (cfgFile.is_open())
    {
        while ( cfgFile.good() )
        {
            getline (cfgFile,line);
            int posPoint= line.find(':',0);
            line = line.substr(posPoint+1,line.length());

            //transforme le string en nombre
            int number;
            stringstream convert(line);
            if ( !(convert >> number) )
                number = 0;
            vctResult.push_back(number);
        }
        cfgFile.close();

        GRID_WIDTH = vctResult[0];
        GRID_HEIGHT = vctResult[1];

        GAME_SCREEN_WIDTH = SPRITE_WIDTH*GRID_WIDTH;
        GAME_SCREEN_HEIGHT = SPRITE_WIDTH*GRID_HEIGHT;
        SCREEN_WIDTH = GAME_SCREEN_WIDTH+150;
        SCREEN_HEIGHT = GAME_SCREEN_HEIGHT;

        INTERF_X = SCREEN_WIDTH-INTERF_WIDTH-20;

        INTERF_MAIN_PIECE_X = INTERF_X - INTERF_WIDTH_BORDER + INTERF_WIDTH /2;
        INTERF_MAIN_PIECE_Y = INTERF_Y + INTERF_WIDTH_BORDER + SPRITE_WIDTH + 3;
    }
}

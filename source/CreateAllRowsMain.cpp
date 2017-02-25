#include <iostream>
#include <fstream>
#include "Game.hpp"
#include "Algorithm.hpp"


using namespace std;
using namespace lottery;


int main()
{
    //load the game
    Game game;
    if (!game.load("Game.csv"))
    {
        std::cerr << "ERROR: The game could not be loaded.\n";
        return -1;
    }

    system("pause");
    return 0;
}


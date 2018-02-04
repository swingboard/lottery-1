#include <iostream>
#include "Game.hpp"


using namespace std;
using namespace Lottery;


int main()
{
    Game game;

    //load the game
    try
    {
        game.load();
    }
    catch (const std::runtime_error &error)
    {
        cout << "Error: " << error.what() << endl;
        return -1;
    }

    return 0;
}

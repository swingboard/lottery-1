#include <iostream>
#include "Game.hpp"


int main()
{
    lottery::Game game;
    if (!game.load())
    {
        std::cerr << "ERROR: The game could not be loaded.\n";
        return -1;
    }

    return 0;
}

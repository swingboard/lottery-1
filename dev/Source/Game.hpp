#ifndef LOTTERY_GAME_HPP
#define LOTTERY_GAME_HPP


#include <string>


namespace Lottery
{


    /**
        Definition of a Lottery game.
        Also contains the game's draws.
     */
    class Game
    {
    public:
        /**
            Loads the game and the draws
            from the current folder.
            @param game filename of the game definition file.
            @param draws filename of the draws.
            @exception std::runtime_exception if there was an error.
         */
        void load(
            const std::string &game = "Game.csv",
            const std::string draws = "Draws.csv");
    };


} //namespace Lottery


#endif //LOTTERY_GAME_HPP

#ifndef LOTTERY_GAME_HPP
#define LOTTERY_GAME_HPP


#include <vector>
#include "SubGame.hpp"


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

        ///returns the subgames of this game.
        const std::vector<SubGame> &getSubGames() const
        {
            return m_subGames;
        }

        ///Returns the number of loaded draws.
        size_t getDrawsCount() const
        {
            return m_subGames.empty() ? 0 : m_subGames[0].m_draws.size();
        }

        /**
            Returns total count of numbers.
         */
        size_t getNumberCount() const
        {
            size_t result = 0;
            for (const SubGame &subGame : m_subGames)
            {
                result += subGame.getNumberCount();
            }
            return result;
        }

    private:
        std::vector<SubGame> m_subGames;
        size_t m_numberCount = 0;
    };


} //namespace Lottery


#endif //LOTTERY_GAME_HPP

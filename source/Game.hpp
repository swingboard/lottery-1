#ifndef LOTTERY_GAME_HPP
#define LOTTERY_GAME_HPP


#include "CSVInputFileStream.hpp"
#include "SubGame.hpp"


namespace lottery
{


    /**
        A class that represents a game.
        A game consists of subgames,
        and each subgame consists of definitions
        and results.
     */
    class Game
    {
    public:
        /**
            Returns the sub-games this game is composed of.
            @return the sub-games this game is composed of.
         */
        const std::vector<SubGame> &getSubGames() const
        {
            return m_subGames;
        }

        /**
            Loads the game from a disk file.
            @param game filename name of the game.
            @param resultsFilename name of the results data.
            @return true if loaded successfully, false otherwise.
         */
        bool load(
            const std::string &gameFilename,
            const std::string &resultsfilename);

        /**
            Loads the game from files 'Game.csv' and 'Results.csv'.
            @return true if loaded successfully, false otherwise.
         */
        bool load()
        {
            return load("Game.csv", "Results.csv");
        }

        /**
            Predicts next draw's numbers.
            @param minPredictedNumbersPerColumn minimum number 
                   of predicted numbers per column.
            @return set of predicted numbers.
         */
        std::set<Number> predictNumbers(
            size_t minPredictedNumbersPerColumn) const;

    private:
        std::vector<SubGame> m_subGames;
    };


} //namespace lottery


#endif //LOTTERY_GAME_HPP

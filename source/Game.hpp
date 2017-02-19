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
            const std::string &resultsfilename)
        {
            //the game file
            CSVInputFileStream gameFile(gameFilename);
            if (!gameFile.is_open())
            {
                return false;
            }

            //the results file
            CSVInputFileStream resultsFile(resultsfilename);
            if (!resultsFile.is_open())
            {
                return false;
            }

            //skip the headers
            for (int index = 0; index < 4; ++index)
            {
                std::string header;
                gameFile >> header;
            }

            m_subGames.clear();

            //read the subgames
            for (;;)
            {
                //read the name
                std::string subGameName;
                gameFile >> subGameName;
                if (gameFile.eof())
                {
                    break;
                }

                //read the min number
                int minNumber;
                gameFile >> minNumber;

                //read the max number
                int maxNumber;
                gameFile >> maxNumber;

                //read the column count
                size_t columnCount;
                gameFile >> columnCount;

                //check if the sub-game was correctly read
                if (!gameFile.good())
                {
                    return false;
                }

                //add a subgame
                m_subGames.emplace_back();
                m_subGames.back().set(
                    subGameName, 
                    (Number)minNumber, 
                    (Number)maxNumber, 
                    columnCount);
            }

            //skip the headers
            for (size_t subGameIndex = 0;
                subGameIndex < m_subGames.size();
                ++subGameIndex)
            {
                for (size_t columnIndex = 0;
                    columnIndex < m_subGames[subGameIndex].m_columnCount;
                    ++columnIndex)
                {
                    {
                        std::string header;
                        resultsFile >> header;
                    }
                }
            }

            //read the numbers
            for (;;)
            {
                for (size_t subGameIndex = 0; 
                    subGameIndex < m_subGames.size(); 
                    ++subGameIndex)
                {
                    for (size_t columnIndex = 0; 
                        columnIndex < m_subGames[subGameIndex].m_columnCount; 
                        ++columnIndex)
                    {
                        int number;
                        resultsFile >> number;
                        if (resultsFile.eof())
                        {
                            return true;
                        }
                        m_subGames[subGameIndex].m_results[columnIndex].push_back(number);
                    }
                    if (!resultsFile.good())
                    {
                        return false;
                    }
                }
            }

            //success
            return true;
        }

        /**
            Loads the game from files 'Game.csv' and 'Results.csv'.
            @return true if loaded successfully, false otherwise.
         */
        bool load()
        {
            return load("Game.csv", "Results.csv");
        }

    private:
        std::vector<SubGame> m_subGames;
    };


} //namespace lottery


#endif //LOTTERY_GAME_HPP

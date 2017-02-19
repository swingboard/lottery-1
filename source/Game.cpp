#include "Game.hpp"


namespace lottery
{


    /**
        Loads the game from a disk file.
        @param game filename name of the game.
        @param resultsFilename name of the results data.
        @return true if loaded successfully, false otherwise.
     */
    bool Game::load(
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
                columnIndex < m_subGames[subGameIndex].getColumnCount();
                ++columnIndex)
            {
                {
                    std::string header;
                    resultsFile >> header;
                }
            }
        }

        //prepare the results vectors
        std::vector<std::vector<Column>> results(m_subGames.size());
        for (size_t subGameIndex = 0;
            subGameIndex < m_subGames.size();
            ++subGameIndex)
        {
            results[subGameIndex].resize(m_subGames[subGameIndex].getColumnCount());
        }

        //read the numbers
        for (;;)
        {
            for (size_t subGameIndex = 0; 
                subGameIndex < m_subGames.size(); 
                ++subGameIndex)
            {
                for (size_t columnIndex = 0; 
                    columnIndex < m_subGames[subGameIndex].getColumnCount(); 
                    ++columnIndex)
                {
                    int number;
                    resultsFile >> number;
                    if (resultsFile.eof())
                    {
                        return true;
                    }
                    results[subGameIndex][columnIndex].push_back(number);
                }
                if (!resultsFile.good())
                {
                    return false;
                }
            }

            for (size_t subGameIndex = 0;
                subGameIndex < m_subGames.size();
                ++subGameIndex)
            {
                m_subGames[subGameIndex].setResults(results[subGameIndex]);
            }
        }

        //success
        return true;
    }


    /**
        Predicts next draw's numbers.
        @param minPredictedNumbersPerColumn minimum number 
                of predicted numbers per column.
        @return set of predicted numbers.
     */
    std::set<Number> Game::predictNumbers(
        size_t minPredictedNumbersPerColumn) const
    {
        //the result
        std::set<Number> predictedNumbers;

        //predict numbers for each sub game
        for (const SubGame &subGame : m_subGames)
        {
            //get the predicted numbers from the sub-game
            std::set<Number> subGamePredictedNumbers = 
                subGame.predictNumbers(minPredictedNumbersPerColumn);

            //add the predicted numbers of the sub-game to the result
            predictedNumbers.insert(
                subGamePredictedNumbers.begin(), 
                subGamePredictedNumbers.end());
        }

        return predictedNumbers;
    }


} //namespace lottery

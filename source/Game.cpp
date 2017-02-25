#include "Game.hpp"


namespace lottery
{


    /**
        Loads the game from a disk file.
        @param game filename name of the game.
        @param resultsFilename name of the results data; if empty, no results are loaded.
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

        //do not load the results if the results filename is empty.
        if (resultsfilename.empty())
        {
            return true;
        }

        //the results file
        CSVInputFileStream resultsFile(resultsfilename);
        if (!resultsFile.is_open())
        {
            return false;
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
                        goto RESULTS_LOADED;
                    }
                    results[subGameIndex][columnIndex].push_back(number);
                }
                if (!resultsFile.good())
                {
                    return false;
                }
            }
        }

        RESULTS_LOADED:

        //set the results of each sub-game
        for (size_t subGameIndex = 0;
            subGameIndex < m_subGames.size();
            ++subGameIndex)
        {
            m_subGames[subGameIndex].setResults(results[subGameIndex]);
        }

        //success
        return true;
    }


    /**
        Predicts next draw's numbers.
        @param minPredictedNumbersPerColumn minimum number of predicted numbers per column.
        @param startResultsIndex begin index of data to examine.
        @param endResultIndex end index of data to examine; if 0, it means the end of data.
        @return set of predicted numbers.
     */
    std::vector<std::vector<Number>> Game::predictNumbers(
        size_t minPredictedNumbersPerColumn,
        size_t startResultsIndex/* = 0*/,
        size_t endResultsIndex/* = 0*/) const
    {
        //the result
        std::vector<std::vector<Number>> predictedNumbers;

        //predict numbers for each sub game
        for (const SubGame &subGame : m_subGames)
        {
            //get the predicted numbers from the sub-game
            std::set<Number> subGamePredictedNumbers = 
                subGame.predictNumbers(minPredictedNumbersPerColumn, startResultsIndex, endResultsIndex);

            //add the predicted numbers of the sub-game to the result
            predictedNumbers.emplace_back();
            predictedNumbers.back().insert(
                predictedNumbers.back().end(),
                subGamePredictedNumbers.begin(), 
                subGamePredictedNumbers.end());
        }

        return predictedNumbers;
    }


} //namespace lottery

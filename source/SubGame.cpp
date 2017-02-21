#include "SubGame.hpp"
#include "Algorithm.hpp"


namespace lottery
{


    /**************************************************************************
        PUBLIC
     **************************************************************************/


    /**
        Sets the subgame data.
        @param name name of the subgame.
        @param minNumber minimum number.
        @param maxNumber maximum number.
        @param columnCount number of columns.
        @exception std::invalid_argument thrown if the arguments are invalid.
     */
    void SubGame::set(
        const std::string &name,
        Number minNumber, 
        Number maxNumber, 
        size_t columnCount)
    {
        //check the args
        if (minNumber > maxNumber)
        {
            throw std::invalid_argument("minNumber greater than maxNumber");
        }
        if (columnCount == 0)
        {
            throw std::invalid_argument("columnCount should not equal zero");
        }

        //set the data
        m_name = name;
        m_minNumber = minNumber;
        m_maxNumber = maxNumber;
        m_columnCount = columnCount;
        m_results.resize(columnCount);
    }


    /**
        Sets the results.
        @param results the sub-game's results.
        @exception std::invalid_argument thrown if the results argument has
            a size different than the number of columns.
     */
    void SubGame::setResults(const std::vector<Column> &results)
    {
        //check that enough columns are provided
        if (results.size() != m_columnCount)
        {
            throw std::invalid_argument("results size different than column count");
        }

        //check that all columns have the same size
        for (size_t index = 1; index < results.size(); ++index)
        {
            if (results[index].size() != results[index - 1].size())
            {
                throw std::invalid_argument("not all results columns have the same size");
            }
        }

        //cache the results size for quick access
        m_resultsSize = m_results[0].size();

        //set the results
        m_results = results;
    }


    /**
        Predicts next draw's numbers.
        @param minPredictedNumbersPerColumn minimum number 
            of predicted numbers per column.
        @return set of predicted numbers.
     */
    std::set<Number> SubGame::predictNumbers(
        size_t minPredictedNumbersPerColumn) const
    {
        return
            getColumnCount() > 1 ?
            _predictNumbersMultiColumn(minPredictedNumbersPerColumn) :
            _predictNumbersSingleColumn(minPredictedNumbersPerColumn);
    }


    /**************************************************************************
        PRIVATE
     **************************************************************************/


    //the algorithm to predict numbers using multiple columns
    std::set<Number> SubGame::_predictNumbersMultiColumn(const size_t minPredictedNumbersPerColumn) const
    {
        //check the arguments
        if (minPredictedNumbersPerColumn == 0)
        {
            throw std::invalid_argument("minPredictedNumbersPerColumn shall not be zero");
        }

        const size_t maxSuccess = 10;
        std::vector<std::unordered_map<size_t, size_t>> totalSuccesses(maxSuccess);
        std::vector<double> predictedNumberCounts(maxSuccess);
        const size_t startResultRowIndex = getRowCount() - 100;
        const size_t endResultRowIndex = getRowCount() - 1;
        RandomNumberGenerator<Number> randomNumber(m_minNumber, m_maxNumber);
        std::vector<std::unordered_map<size_t, size_t>> totalRandomSuccesses(maxSuccess);

        for (size_t resultIndex = startResultRowIndex; resultIndex < endResultRowIndex; ++resultIndex)
        {
            std::vector<std::vector<Number>> partialResults(m_columnCount);
            for (size_t columnIndex = 0; columnIndex < m_columnCount; ++columnIndex)
            {
                partialResults[columnIndex] = std::vector<Number>(m_results[columnIndex].begin(), m_results[columnIndex].begin() + resultIndex);
            }

            //calculate the number transitions
            std::vector<std::vector<std::pair<Number, double>>> numberTransitions(m_columnCount);
            for (size_t columnIndex = 0; columnIndex < m_columnCount; ++columnIndex)
            {
                calculateTransitions(partialResults[columnIndex], 1, 0, numberTransitions[columnIndex]);
            }

            //calculate the numbers' average transitions
            std::vector<double> sums;
            std::vector<std::vector<double>> columnAverages = calculateColumnAverages(partialResults, sums);
            std::vector<std::vector<std::pair<double, double>>> columnAverageTransitions(m_columnCount);
            for (size_t columnIndex = 0; columnIndex < m_columnCount; ++columnIndex)
            {
                calculateTransitions(columnAverages[columnIndex], 1, 0.001, columnAverageTransitions[columnIndex]);
            }

            //using the number transitions and average transitions, find the most probable transitions
            const size_t maxRows = resultIndex + 1;
            std::vector<std::vector<std::pair<Number, double>>> bestNumberTransitions(m_columnCount);
            for (size_t columnIndex = 0; columnIndex < m_columnCount; ++columnIndex)
            {
                for (size_t numberIndex = 0; numberIndex < numberTransitions[columnIndex].size(); ++numberIndex)
                {
                    const Number number = numberTransitions[columnIndex][numberIndex].first;
                    const double sum = sums[columnIndex] + number;
                    const double average = sum / maxRows;
                    const double match = evaluateTransition(average, columnAverageTransitions[columnIndex]);
                    bestNumberTransitions[columnIndex].push_back(std::make_pair(number, match * numberTransitions[columnIndex][numberIndex].second));
                }
                std::sort(
                    bestNumberTransitions[columnIndex].begin(),
                    bestNumberTransitions[columnIndex].end(),
                    TupleMemberComparator<std::greater<double>, 1>());
            }

            std::vector<size_t> successes(maxSuccess);
            std::vector<size_t> randomSuccesses(maxSuccess);
            std::unordered_set<Number> predictedNumbers;
            for (size_t i = 0; i < maxSuccess; ++i)
            {
                for (size_t columnIndex = 0; columnIndex < m_columnCount; ++columnIndex)
                {
                    if (i < bestNumberTransitions[columnIndex].size())
                    {
                        predictedNumbers.insert(bestNumberTransitions[columnIndex][i].first);
                    }
                }

                predictedNumberCounts[i] += predictedNumbers.size();

                std::unordered_set<Number> randomNumbers = createRandomNumbers(randomNumber, predictedNumbers.size());

                for (size_t columnIndex = 0; columnIndex < m_columnCount; ++columnIndex)
                {
                    if (predictedNumbers.find(m_results[columnIndex][resultIndex + 1]) != predictedNumbers.end())
                    {
                        ++successes[i];
                    }

                    if (randomNumbers.find(m_results[columnIndex][resultIndex + 1]) != randomNumbers.end())
                    {
                        ++randomSuccesses[i];
                    }
                }
            }

            for (size_t i = 0; i < maxSuccess; ++i)
            {
                ++totalSuccesses[i][successes[i]];
                ++totalRandomSuccesses[i][randomSuccesses[i]];
            }
        }

        std::ofstream file("data.txt");
        const size_t maxRows = endResultRowIndex - startResultRowIndex;
        for (size_t i = 0; i < maxSuccess; ++i)
        {
            file << "Depth " << (i + 1) << " : \n";
            for (size_t success = 1; success <= m_columnCount; ++success)
            {
                file << "    success of " << success << " : ";
                file << std::setw(7) << std::fixed << std::setprecision(3) << (100.0*percent(totalSuccesses[i][success], maxRows)) << "% vs ";
                file << std::setw(7)  << std::fixed << std::setprecision(3) << (100.0*percent(totalRandomSuccesses[i][success], maxRows)) << "%\n";
            }
            file << "    average count of predicted numbers per row = ";
            const double predictedNumberCountPerRow = predictedNumberCounts[i] / maxRows;
            file << std::fixed << std::setprecision(3) << predictedNumberCountPerRow << '\n';
            file << '\n';
        }
        file << '\n';

        std::set<Number> predictedNumbers;
        return predictedNumbers;
    }


    //the algorithm to predict numbers using one column
    std::set<Number> SubGame::_predictNumbersSingleColumn(size_t minPredictedNumbersPerColumn) const
    {
        //the result
        std::set<Number> predictedNumbers;

        //TODO
    
        return predictedNumbers;
    }


} //namespace lottery

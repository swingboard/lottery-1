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

        //for all columns, calculate the most transitions for the last results
        //to the next ones and their probabilities, based on prior data.
        std::vector<std::vector<std::pair<Number, double>>> nextNumbers(m_columnCount);
        for (size_t columnIndex = 0; columnIndex < m_columnCount; ++columnIndex)
        {
            calculateTransitions(m_results[columnIndex], 1, nextNumbers[columnIndex]);
        }

        //calculate all possible draws and their probabilities from next possible numbers;
        //process each draw according to the following criteria:
        //TODO

        //temp buffer
        std::vector<Number> temp(m_columnCount);

        //calculate all possible draws from next numbers
        calculatePermutations(nextNumbers, 
            [&](const std::vector<std::pair<Number, double>> &values)
            {
                //transform the array of values to an array of numbers
                std::transform(values.begin(), values.end(), temp.begin(), 
                    [](const std::pair<Number, double> &v) { return v.first; });

                //sort the numbers
                std::sort(temp.begin(), temp.begin() + m_columnCount, std::less<Number>());

                //the numbers must be unique
                if (!hasUniqueValues(temp.begin(), temp.begin() + m_columnCount))
                {
                    return;
                }

                //compute probability of draw
                const double probability = 
                    std::accumulate(values.begin(), values.end(), 1.0, 
                        [](double probability, const std::pair<Number, double> &value)
                            { return probability * value.second; });

                //process the draw
                //TODO
            });

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

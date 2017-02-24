#include "SubGame.hpp"
#include "Statistics.hpp"


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

        //set the results
        m_results = results;

        //cache the results size for quick access
        m_resultsSize = m_results[0].size();
    }


    /**
        Predicts next draw's numbers.
        @param minPredictedNumbersPerColumn minimum number of predicted numbers per column.
        @param startResultsIndex begin index of data to examine.
        @param endResultIndex end index of data to examine; if 0, it means the end of data.
        @return set of predicted numbers.
     */
    std::set<Number> SubGame::predictNumbers(
        size_t minPredictedNumbersPerColumn,
        size_t startResultsIndex,
        size_t endResultsIndex) const
    {
        //autodetect end index
        if (endResultsIndex == 0)
        {
            endResultsIndex = getRowCount();
        }

        //depending on if the subgame has one or multiple columns, use different algorithms
        return
            getColumnCount() > 1 ?
            _predictNumbersMultiColumn(minPredictedNumbersPerColumn, startResultsIndex, endResultsIndex) :
            _predictNumbersSingleColumn(minPredictedNumbersPerColumn, startResultsIndex, endResultsIndex);
    }


    /**************************************************************************
        PRIVATE
     **************************************************************************/


    //the algorithm to predict numbers using multiple columns
    std::set<Number> SubGame::_predictNumbersMultiColumn(
        const size_t minPredictedNumbersPerColumn,
        size_t startResultsIndex,
        size_t endResultsIndex) const
    {
        //check the arguments
        if (minPredictedNumbersPerColumn == 0)
        {
            throw std::invalid_argument("minPredictedNumbersPerColumn shall not be zero");
        }

        //TODO    
        std::set<Number> predictedNumbers;
        return predictedNumbers;
    }


    //the algorithm to predict numbers using one column
    std::set<Number> SubGame::_predictNumbersSingleColumn(
        size_t minPredictedNumbersPerColumn,
        size_t startResultsIndex,
        size_t endResultsIndex) const
    {
        //TODO    
        std::set<Number> predictedNumbers;
        return predictedNumbers;
    }


} //namespace lottery

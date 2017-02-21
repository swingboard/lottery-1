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

        //column sums
        std::vector<int> columnSums(m_columnCount);

        //column averages
        std::vector<double> columnAverages(m_columnCount);

        //row sums
        std::vector<int> rowSums(getRowCount() - 1);

        //row delta sums
        std::vector<int> rowDeltaSums(getRowCount() - 1);

        //iterate all rows except the last one, since the prediction is for the row next to the current one.
        for (size_t rowIndex = 0; rowIndex < getRowCount() - 1; ++rowIndex)
        {
            //compute column sums and averages; 
            //also compute sum of row
            for (size_t columnIndex = 0; columnIndex < m_columnCount; ++columnIndex)
            {
                const Number number = m_results[columnIndex][rowIndex];
                columnSums[columnIndex] += number;
                columnAverages[columnIndex] = columnSums[columnIndex] / (double)(rowIndex + 1);
                rowSums[rowIndex] += number;
            }

            //compute delta sums of row
            for (size_t columnIndex = 0; columnIndex < m_columnCount - 1; ++columnIndex)
            {
                const Number number1 = m_results[columnIndex][rowIndex];
                const Number number2 = m_results[columnIndex + 1][rowIndex];
                rowDeltaSums[rowIndex] += number2 - number1;
            }
        }

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

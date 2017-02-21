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

        //column deltas
        std::vector<std::vector<int>> columnDeltas(m_columnCount, std::vector<int>(getRowCount() - 1));

        //row sums
        std::vector<int> rowSums(getRowCount() - 1);

        //row delta sums
        std::vector<int> rowDeltaSums(getRowCount() - 1);

        //row deltas
        std::vector<std::vector<int>> rowDeltas(m_columnCount, std::vector<int>(getRowCount() - 1));

        //iterate all rows except the last one, since the prediction is for the row next to the current one;
        //also skip the first row because the column deltas need to be computed.
        for (size_t rowIndex = 1; rowIndex < getRowCount() - 1; ++rowIndex)
        {
            //compute column sums and averages; 
            //also compute sum of row;
            //also compute column deltas.
            for (size_t columnIndex = 0; columnIndex < m_columnCount; ++columnIndex)
            {
                const int number = m_results[columnIndex][rowIndex];

                //calculate column sums
                columnSums[columnIndex] += number;

                //calculate column averages
                columnAverages[columnIndex] = columnSums[columnIndex] / (double)(rowIndex + 1);

                //calculate row sums
                rowSums[rowIndex] += number;

                //compute column deltas
                const int prevNumber = m_results[columnIndex][rowIndex - 1];
                columnDeltas[columnIndex][rowIndex] = number - prevNumber;
            }

            //compute delta sums of row;
            //also compute row deltas.
            for (size_t columnIndex = 0; columnIndex < m_columnCount - 1; ++columnIndex)
            {
                const Number number1 = m_results[columnIndex][rowIndex];
                const Number number2 = m_results[columnIndex + 1][rowIndex];
                const int delta = number2 - number1;

                //compute delta sums of row
                rowDeltaSums[rowIndex] += delta;

                //compute row deltas
                rowDeltas[columnIndex][rowIndex] = delta;
            }

            /*
                predict the next row by this criteria:
                -each number of the predicted row does not create a column average that deviates too much from the predicted column average.
                -the sum of the numbers of the predicted row does not deviate too much from the predicted row sum.
                -the delta of each number of the predicted row from its previous number of the same column does not deviate too much from the predicted column delta. 
                -the sum of the deltas between the numbers of the predicted row does not deviate too much from the predicted sum of deltas.
                -the deltas between the numbers of the predicted row do not deviate too much from the predicted deltas.
            */
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

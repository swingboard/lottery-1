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

        //cache the results size for quick access
        m_resultsSize = m_results[0].size();

        //set the results
        m_results = results;
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

        //number of samples
        const size_t sampleCount = endResultsIndex - startResultsIndex;

        //predicted numbers per column, accompanied with their probability.
        std::vector<std::unordered_map<Number, double>> candidateNumbersPerColumn(m_columnCount);

        //create per-column statistics useful in the prediction
        for (size_t columnIndex = 0; columnIndex < m_columnCount; ++columnIndex)
        {
            //column results
            const std::vector<Number> &columnResults = m_results[columnIndex];

            //last column result
            const Number lastNumber = columnResults[endResultsIndex - 1];

            //next numbers of the last column result
            std::unordered_map<Number, size_t> nextNumbers;

            //next number count
            size_t nextNumberCount = 0;

            //previous number
            Number previousNumber = 0;

            //sum of numbers of column
            size_t numberSum = 0;

            //average of numbers of column
            double numberAverage = 0;

            //average deltas
            std::vector<double> averageDeltas;

            //iterate the given rows
            for (size_t rowIndex = startResultsIndex; rowIndex < endResultsIndex; ++rowIndex)
            {
                //number at current row
                const Number number = columnResults[rowIndex];

                //if the previous number equals the last number, then add the current number
                //to the set of next numbers
                if (previousNumber == lastNumber)
                {
                    ++nextNumbers[number];
                    ++nextNumberCount;
                }
                previousNumber = number;

                //update the sum of numbers
                numberSum += number;

                //new average of numbers
                const size_t currentSampleCount = rowIndex - startResultsIndex + 1;
                const double newNumberAverage = numberSum / (double)currentSampleCount;

                //delta of the new average from the last one
                const double newNumberAverageDelta = std::abs(newNumberAverage - numberAverage);

                //update the number average
                numberAverage = newNumberAverage;

                //update the average deltas
                averageDeltas.push_back(newNumberAverageDelta);
            }

            //calculate the sum of last N average deltas
            double numberAverageDeltaSum = 0;
            const size_t averageDeltaSampleCount = std::min(100U, averageDeltas.size());
            for (size_t averageDeltaIndex = averageDeltas.size() - averageDeltaSampleCount; 
                averageDeltaIndex < averageDeltas.size(); 
                ++averageDeltaIndex)
            {
                numberAverageDeltaSum += averageDeltas[averageDeltaIndex];
            }

            //calculate the final number average delta average
            const double numberAverageDeltaAverage = numberAverageDeltaSum / (double)averageDeltaSampleCount;

            //process each next number found against the average;
            //if the number does not modify the average over a certain limit,
            //then consider the number as a good candidate for being the next number
            for (const auto &nextNumberPair : nextNumbers)
            {
                //the number
                const Number nextNumber = nextNumberPair.first;

                //compute a new sum with the current next number
                const double newNumberSum = numberSum + nextNumber;

                //compute a new average with the current next number
                const double newNumberAverage = newNumberSum / (double)(sampleCount + 1);

                //compute a new average delta
                const double newNumberAverageDelta = std::abs(newNumberAverage - numberAverage);

                //add the next number as a candidate if the new delta is below the delta average
                if (newNumberAverageDelta <= numberAverageDeltaAverage)
                {
                    //the number of occurrences
                    const size_t nextNumberOccurrences = nextNumberPair.second;

                    //the probability of occurrence of the next number
                    const double probability = nextNumberOccurrences / (double)nextNumberCount;

                    //add the number to the candidate numbers per column
                    candidateNumbersPerColumn[columnIndex][nextNumber] = probability;
                }
            }
        }

        //TODO    
        std::set<Number> predictedNumbers;
        for (const auto &map : candidateNumbersPerColumn)
        {
            for (const auto &p : map)
            {
                predictedNumbers.insert(p.first);
            }
        }
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

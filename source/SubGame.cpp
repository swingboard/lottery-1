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

        //the transitions of numbers per column
        std::vector<TransitionMap<Number>> perColumn_numberTransitions(m_columnCount);
        for (size_t columnIndex = 0; columnIndex < m_columnCount; ++columnIndex)
        {
            perColumn_numberTransitions[columnIndex] = createTransitionMap(m_results[columnIndex]);
        }

        //the transitions of sums of each draw
        const int drawnNumberSumQuantum = 10;
        std::vector<int> perRow_drawnNumberSums = sumColumns(m_results);
        std::vector<int> perRow_drawnNumberSums_quantized = quantize(perRow_drawnNumberSums, drawnNumberSumQuantum);
        const int drawnNumberSums_range = range(range(perRow_drawnNumberSums_quantized));
        TransitionMap<int> drawnNumberSumsTransitions = createTransitionMap(perRow_drawnNumberSums_quantized);

        /*
            Try to find, per column, the numbers that match most all the following criteria:
            1) most probable number transitions.
            2) most probable drawn column sum transitions.
         */

         //latest drawn numbers' transitions
        std::vector<std::vector<Number>> perColumn_latestDrawnNumbers_transitions(m_columnCount);
        for (size_t columnIndex = 0; columnIndex < m_columnCount; ++columnIndex)
        {
            Number latestDrawnNumber = m_results[columnIndex].back();
            perColumn_latestDrawnNumbers_transitions[columnIndex] = 
                valuesOf(perColumn_numberTransitions[columnIndex][latestDrawnNumber]);
        }        

        //latest drawn number sum transitions 
        std::vector<int> latestdrawnNumberSum_transitions = 
            valuesOf(drawnNumberSumsTransitions[perRow_drawnNumberSums_quantized.back()]);

        //scores of combinations of score, numbers, sum
        std::vector<std::tuple<double, std::vector<Number>, int>> matches;

        //find the best match of transitions of numbers to transitions of result sums
        iterate(latestdrawnNumberSum_transitions, [&](int nextSum)
        {
            iterate(perColumn_latestDrawnNumbers_transitions, [&](const std::vector<Number> &nextNumbers)
            {
                const int nextNumbersSum = quantize(sumValues<int>(nextNumbers), drawnNumberSumQuantum);
                const int sumDelta = std::abs(nextNumbersSum - nextSum);
                const double score = percent(sumDelta, drawnNumberSums_range);
                matches.push_back(std::make_tuple(score, nextNumbers, nextSum));
            });
        });

        //sort the scores in ascending order
        sort(matches, TupleMemberComparator<std::greater<double>, 0>());

        //create the predicted numbers
        const auto &bestMatch = matches[0];
        const std::vector<Number> &bestMatchNumbers = std::get<1>(bestMatch);
        std::set<Number> predictedNumbers;
        const int subFromNumber = static_cast<int>(minPredictedNumbersPerColumn) / 2;
        for (const Number number : bestMatchNumbers)
        {
            for (int i = 0; i < static_cast<int>(minPredictedNumbersPerColumn); ++i)
            {
                const Number predictedNumber = static_cast<Number>(number + i - subFromNumber);
                if (inRange(predictedNumber, m_minNumber, m_maxNumber))
                {
                    predictedNumbers.insert(predictedNumber);
                }
            }
        }

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

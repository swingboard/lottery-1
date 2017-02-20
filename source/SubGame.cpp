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
        const int drawnNumberSumQuantum = 1;
        std::vector<int> perRow_drawnNumberSums = sumColumns(m_results);
        std::vector<int> perRow_drawnNumberSums_quantized = quantize(perRow_drawnNumberSums, drawnNumberSumQuantum);
        const int drawnNumberSums_range = range(range(perRow_drawnNumberSums_quantized));
        TransitionMap2<int> drawnNumberSumsTransitions_quantized = createTransitionMap2(perRow_drawnNumberSums_quantized);

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
        const int prevSum2 = *(perRow_drawnNumberSums_quantized.end() - 2);
        const int prevSum1 = *(perRow_drawnNumberSums_quantized.end() - 1);
        std::vector<int> latestdrawnNumberSum_transitions_quantized = 
            valuesOf(drawnNumberSumsTransitions_quantized[prevSum2][prevSum1]);

        //scores of combinations of score, numbers
        typedef std::tuple<double, std::vector<Number>> Match;

        //min limit to place a combination in the matches
        const double minScore = 0.99;

        //top matches
        const size_t topMatchCount = 10;
        std::vector<Match> topMatches(topMatchCount);

        //find the best match of transitions of numbers to transitions of result sums
        iterate(latestdrawnNumberSum_transitions_quantized, [&](int nextSum_quantized)
        {
            return iterate(perColumn_latestDrawnNumbers_transitions, [&](const std::vector<Number> &nextNumbers)
            {
                std::vector<Number> sortedNumbers(nextNumbers);
                sort(sortedNumbers, std::less<Number>());
                if (!containsDuplicates(sortedNumbers))
                {
                    const int nextNumbersSum = sumValues<int>(sortedNumbers);
                    const int nextNumbersSum_quantized = quantize(nextNumbersSum, drawnNumberSumQuantum);
                    const int sumDelta = std::abs(nextNumbersSum_quantized - nextSum_quantized);
                    const double score = percent(sumDelta, drawnNumberSums_range);
                    if (score >= minScore)
                    {
                        const Match match = std::make_tuple(score, sortedNumbers);
                        insertSorted(topMatches, match, TupleMemberComparator<std::greater<double>, 0>());
                        if (topMatches.size() > topMatchCount)
                        {
                            topMatches.pop_back();
                        }
                    }
                }
                return true;
            });
        });

        //create the predicted numbers
        std::set<Number> predictedNumbers;
        for (size_t i = 0; i < minPredictedNumbersPerColumn; ++i)
        {
            const Match &match = topMatches[i];
            const std::vector<Number> &matchNumbers = std::get<1>(match);
            for (const Number number : matchNumbers)
            {
                predictedNumbers.insert(number);
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

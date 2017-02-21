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

        //calculate the most probable transitions for numbers
        std::vector<std::vector<std::pair<Number, double>>> nextNumbers(m_columnCount);
        for (size_t columnIndex = 0; columnIndex < m_columnCount; ++columnIndex)
        {
            calculateTransitions(m_results[columnIndex], 1, 0, nextNumbers[columnIndex]);
        }

        //calculate the most probable transitions for sums of deltas between numbers
        std::vector<int> deltaSums = calculateDeltaSums<int>(m_results);
        std::vector<std::pair<int, double>> deltaSumsTransitions;
        calculateTransitions(deltaSums, 1, 0, deltaSumsTransitions);
        std::vector<int> nextDeltaSums(deltaSumsTransitions.size());
        std::transform(deltaSumsTransitions.begin(), deltaSumsTransitions.end(), nextDeltaSums.begin(), 
            [](const std::pair<int, double> &v) { return v.first; });

        //calculate all possible draws and their probabilities from next possible numbers;
        //process each draw according to the following criteria:
        //1) closest to the next expected sum of deltas between columns

        //temp draw buffer
        union {
            Number numbers[8];
            uint64_t value;
        } tempDraw;

        //sanity check
        if (m_columnCount > sizeof(tempDraw.numbers))
        {
            throw std::logic_error("column count greater than temp buffer size");
        }

        //predicted draws, in compressed format, vs their probability
        std::unordered_map<uint64_t, double> nextDraws;

        //calculate all possible draws from next numbers
        calculatePermutations(nextNumbers, 
            [&](const std::vector<std::pair<Number, double>> &values)
            {
                //transform the array of values to an array of numbers
                Number *restOfNumberEntries = std::transform(values.begin(), values.end(), tempDraw.numbers, 
                    [](const std::pair<Number, double> &v) { return v.first; });
                std::fill(restOfNumberEntries, tempDraw.numbers + 8, 0);

                //sort the numbers
                std::sort(tempDraw.numbers, tempDraw.numbers + m_columnCount, std::less<Number>());

                //the numbers must be unique
                if (!hasUniqueValues(tempDraw.numbers, tempDraw.numbers + m_columnCount))
                {
                    return;
                }

                //get the delta sum of the future column
                const int deltaSum = calculateDeltaSum<int>(tempDraw.numbers, tempDraw.numbers + m_columnCount);

                //compute probability of numbers being drawn
                const double probabilityNumbers = 
                    std::accumulate(values.begin(), values.end(), 1.0, 
                        [](double probability, const std::pair<Number, double> &value)
                            { return probability * value.second; });

                //compute the probability of the closest match
                const std::pair<size_t, double> matchDeltaSum = matchValue(deltaSum, nextDeltaSums);
                const double deltaSumProbability = deltaSumsTransitions[matchDeltaSum.first].second;
                const double probabilityDeltaSum = matchDeltaSum.second * deltaSumProbability;

                //calculate the final probability
                const double probability = probabilityNumbers * probabilityDeltaSum;

                //check out if the draw has already been set
                const auto it = nextDraws.find(tempDraw.value);

                //if found, then compare stored probability against newly computed one;
                //if stored probability greater than newly computed one, do nothing else
                if (it != nextDraws.end() && it->second > probability)
                {
                    return;
                }

                //store the new probability
                nextDraws[tempDraw.value] = probability;
            });

        std::vector<std::pair<double, std::vector<Number>>> nextDrawsSorted;
        nextDrawsSorted.reserve(nextDraws.size());
        std::vector<Number> temp(m_columnCount);
        for (const auto &p : nextDraws)
        {
            tempDraw.value = p.first;
            std::copy(tempDraw.numbers, tempDraw.numbers + m_columnCount, temp.begin());
            nextDrawsSorted.push_back(std::make_pair(p.second, temp));
        }
        std::sort(nextDrawsSorted.begin(), nextDrawsSorted.end(), TupleMemberComparator<std::greater<double>, 0>());

        std::set<Number> predictedNumbers;
        for (size_t i = 0; i < minPredictedNumbersPerColumn; ++i)
        {
            const std::vector<Number> &vec = nextDrawsSorted[i].second;
            for (const Number n : vec)
            {
                predictedNumbers.insert(n);
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

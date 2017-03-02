#include "PredictionAlgorithm_Average.hpp"
#include "String.hpp"


namespace lottery
{


    //iterates a value until it finds that the newly calculated average starts to diverge from the target;
    //returns the number that caused the newly calculated average to converge
    static double _findComplementValue(const double incompleteSum, double testValue, const size_t count, const double targetAverage, const double inc, const double minValue)
    {
        //result set to -1 indicates a failure to find a value which causes the averages to converge
        double result = -1;

        //first step in order to establish the initial difference
        const double sum1 = incompleteSum + testValue;
        const double avg1 = sum1 / count;
        double prevDiff = std::abs(avg1 - targetAverage);
        
        //rest of steps, until the difference of averages diverge or a 0 or negative value is reached
        for(;;)
        {
            //next test value
            testValue += inc;

            //if test value became less than min value, then stop
            if (testValue < minValue) break;

            //compute new difference from average
            const double sum = incompleteSum + testValue;
            const double avg = sum / count;
            const double diff = std::abs(avg - targetAverage);

            //if the averages diverge, stop
            if (diff > prevDiff)
            {
                break;
            }

            //otherwise if the averages converge, note down the test value
            else if (diff < prevDiff)
            {
                result = testValue;
            }

            //keep the previous difference in order to compare it with the next
            prevDiff = diff;
        }

        return result;
    }


    //calculates the value that must be added to the given incomplete sum so as that average(sum + value) is closest to given target average
    static double _findAverageComplement(const double incompleteSum, const double averageValue, const size_t count, const double targetAverage, const double minAllowedValue)
    {
        double minValue, maxValue, value1, value2;
        
        //find the first value that causes the averages to converge 
        value1 = _findComplementValue(incompleteSum, averageValue, count, targetAverage, -1, minAllowedValue);        

        //if the first value is found
        if (value1 >= minAllowedValue)
        {
            //find the second value below the first one that causes the averages to converge
            value2 = _findComplementValue(incompleteSum, value1 - 1, count, targetAverage, -1, minAllowedValue);

            //if the second value is found, then since value2 < value1,
            //the min value is value2, and the max value is value1.
            if (value2 >= minAllowedValue)
            {
                minValue = value2;
                maxValue = value1;
            }

            //else if no value was found below value1 that caused the averages to converge,
            //try a value above the average value
            else
            {
                value2 = _findComplementValue(incompleteSum, averageValue, count, targetAverage, 1, minAllowedValue);

                //if the value is found, then since value2 > value1,
                //the min value is value1, and the max value is value2
                if (value2 >= minAllowedValue)
                {
                    minValue = value1;
                    maxValue = value2;
                }

                //else if not found, it means min and max value are equal,
                //since only one value caused the averages to converge
                else
                {
                    minValue = maxValue = value1;
                }
            }
        }

        //else since the first value was not found below the average value, try above the average value
        else
        {
            value1 = _findComplementValue(incompleteSum, averageValue, count, targetAverage, 1, minAllowedValue);        

            //if the first value is found, try to find the second value above the first one
            if (value1 >= minAllowedValue)
            {
                value2 = _findComplementValue(incompleteSum, value1 + 1, count, targetAverage, 1, minAllowedValue);

                //if value2 is found, then since value2 > value1,
                //then the min value is value1, and the max value is value2
                if (value2 >= minAllowedValue)
                {
                    minValue = value1;
                    maxValue = value2;
                }

                //otherwise set min and max value to value1, since value1 was the only value to make the averages converge
                else
                {
                    minValue = maxValue = value1;
                }
            }

            //else if the first value is not found either below the average value or above the average value,
            //then set the min/max values to the average
            else
            {
                minValue = maxValue = averageValue;
            }
        }

        //return the medium value between min and max
        const double result = medium(minValue, maxValue);
        return result;
    }


    //predict value via searching for value that satisfies the next projected average.
    static double _predictValueHelper(std::vector<double> &values, std::vector<double> &averages, const int minValue, const double averageValue, const size_t count, const size_t depth, const size_t maxDepth)
    {
        double result;

        //sum of end sequence minus one value
        const double incompleteSum = sum(values.end() - (count - 1), values.end());

        //if not in deepest level, calculate the averages of averages until the deepest level is reached
        if (depth > 0)
        {
            const size_t offset = (maxDepth - depth) * (count - 1);

            //calculate the averages of all the values, except the last one, which are stored in the 'values' container;
            //the last value will be calculated from the target average.
            for (auto it = values.begin() + offset, itEnd = values.end() - count + 1, avgIt = averages.begin() + offset + count - 1;
                it != itEnd;
                ++it, ++avgIt)
            {
                const double v = sum(it, it + count) / (double)count;
                *avgIt = v;
            }

            //calculate averages of next level; the next min number is not the lottery min number,
            //but 0 (averages are allowed to be 0, lottery numbers do not).
            std::vector<double> newAverages(averages.size());
            const double targetAverage = _predictValueHelper(averages, newAverages, 0, averageValue, count, depth - 1, maxDepth);
        
            //find the result as the number that satisfies the formula:
            //(incompleteSum + predictedNumber) / count == targetAverage.
            result = _findAverageComplement(incompleteSum, averageValue, count, targetAverage, minValue);
        }

        //else if in deepest level, the result is the average of the incomplete sum.
        else
        {
            result = incompleteSum / (count - 1);
        }

        //return the predicted value
        return result;
    }


    //predict value via searching for value that satisfies the next projected average.
    static double _predictValue(std::vector<double> &values, std::vector<double> &averages, const int minValue, const double averageValue, const size_t count, const size_t depth)
    {
        return _predictValueHelper(values, averages, minValue, averageValue, count, depth, depth);
    }


    /**
        Constructor.
        @param count count of numbers to take the average of.
        @param depth number of averages' sequences to use. 
        @exception std::invalid_argument thrown if count < 2 or depth > count.
        */
    PredictionAlgorithm_Average::PredictionAlgorithm_Average(size_t count, size_t depth)
        : m_count(count)
        , m_depth(depth)
    {
        if (m_count < 2)
        {
            throw std::invalid_argument("count shall not be less than 2");
        }
        if (m_depth > m_count)
        {
            throw std::invalid_argument("depth shall not be greater than or equal to count");
        }
    }


    /**
        Returns the algorithm's name.
     */
    std::string PredictionAlgorithm_Average::getName() const
    {
        return "Average"_s + '_' + m_count + '_' + m_depth;
    }


    /**
        Predicts the next numbers by predicting the averages of current number sequence per column.
        @param game game for which the prediction is for.
        @param draws previous draws.
        @param numberCountPerColumn count of numbers to predict per column.
        @param numbers predicted numbers.
        @exception std::logic_error thrown if the test data are not big enough for the count and depth parameters.
     */
    void PredictionAlgorithm_Average::predict(const Game &game, const DrawVector &draws, const size_t numberCount, std::unordered_set<Number> &numbers)
    {
        //count of values to use
        const size_t previousNumberCount = (m_count - 1) * (m_depth + 1);

        //check if the previous number count is valid
        if (previousNumberCount > draws.size() / 2)
        {
            throw std::logic_error("test data not big enough for count = "_s + m_count + " and depth = " + m_depth);
        }

        //values to calculate the averages of
        std::vector<double> values(previousNumberCount);
        std::vector<double> averages(previousNumberCount);

        //sets of numbers per column
        std::vector<std::vector<Number>> numbersPerColumn(game.numberCount);

        //calculate how many numbers per column to put, rounded for number count
        const size_t numberCountPerColumn = ((numberCount + game.numberCount - 1) / game.numberCount) * 3;

        //this is used to avoid inserting the same numbers twice
        std::unordered_set<Number> tempNumbers;

        //for all columns
        for (size_t columnIndex = 0; columnIndex < game.numberCount; ++columnIndex)
        {
            //for all existing numbers, calculate the values from the last numbers of column
            for (size_t numberIndex = 0; numberIndex < previousNumberCount; ++numberIndex)
            {
                values[numberIndex] = draws[draws.size() - previousNumberCount + numberIndex][columnIndex];
            }

            //the average value of the column is proven to be calculated by the formula:
            //(columnIndex + 1) * (maxNumber / (columnCount + 1))
            const double averageValue = (columnIndex + 1.0) * (game.getMaxNumber(columnIndex) / (double)(game.getNumberCount(columnIndex) + 1.0));

            //cannot predict numbers lower than this
            const Number minNumber = game.getMinNumber(columnIndex);

            //calculate the last value
            const double predictedValue = _predictValue(values, averages, minNumber, averageValue, m_count, m_depth);

            //predicted numbers of the current column
            std::vector<Number> &numbersOfColumn = numbersPerColumn[columnIndex];

            //save the last value as the predicted one for the column
            //TODO is rounding necessary?
            const Number predictedNumber = static_cast<Number>(predictedValue);
            if (tempNumbers.insert(predictedNumber).second) 
            {
                numbersOfColumn.push_back(predictedNumber);
                if (numbersOfColumn.size() == numberCountPerColumn)
                {
                    goto NEXT_COLUMN;
                }
            }

            //calculate numbers that are neighbours to the predicted number
            //until the requested count of predicted numbers for the column is reached
            const Number maxNumber = game.getMaxNumber(columnIndex);
            for (int i = 1; ; ++i)
            {
                const size_t countBeforeInsert = tempNumbers.size();

                //calculate a number smaller than the predicted value
                const Number smallerPredictedNumber = predictedNumber - i;
                if (isMid(minNumber, smallerPredictedNumber, maxNumber) && tempNumbers.insert(smallerPredictedNumber).second)
                {
                    numbersOfColumn.push_back(smallerPredictedNumber);
                    if (numbersOfColumn.size() == numberCountPerColumn)
                    {
                        goto NEXT_COLUMN;
                    }
                }

                //calculate a number bigger than the predicted value
                const Number biggerPredictedNumber = predictedNumber + i;
                if (isMid(minNumber, biggerPredictedNumber, maxNumber) && tempNumbers.insert(biggerPredictedNumber).second)
                {
                    numbersOfColumn.push_back(biggerPredictedNumber);
                    if (numbersOfColumn.size() == numberCountPerColumn)
                    {
                        goto NEXT_COLUMN;
                    }
                }

                //if no value was inserted, stop
                if (tempNumbers.size() == countBeforeInsert)
                {
                    goto NEXT_COLUMN;
                }
            }

            NEXT_COLUMN:
            ;
        }

        //now dispatch the predicted numbers to the result, cyclicly, starting from top rows,
        //until the requested count of numbers is reached
        size_t row = 0;
        while (numbers.size() < numberCount)
        {
            const size_t numbersSizeBefore = numbers.size();

            //iterate rows of predicted numbers and place them in the results; avoid duplicates
            for (size_t columnIndex = 0; columnIndex < game.numberCount && numbers.size() < numberCount; ++columnIndex)
            {
                if (row < numbersPerColumn[columnIndex].size())
                {
                    const Number number = numbersPerColumn[columnIndex][row];
                    numbers.insert(number);
                }
            }

            //detect if no number was added; then break
            if (numbers.size() == numbersSizeBefore) break;

            //next row
            ++row;
        }
    }


} //namespace lottery
